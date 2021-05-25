/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */


#include "motor_control_hal.h"
#include "app.h"
#include "motor_control.h"


//#define COMM_debugPrint(x) printf(x)//comment to disable debug messages
#define COMM_debugPrint(x)//uncomment to disable debug messages

#define ENABLE_DATA_VISUALIZER () //comment to disable data visualizer 

/*Button debounce delay in miliseconds*/
#define BTN_DEBOUNCE_DELAY                  200//multiple of 5

const char* overcurrentMSG = "FAULT: Overcurrent Condition\n";
const char* zeroCrossTimeoutMSG = "FAULT: Zero Cross Timeout\n";
const char* zeroCrossDeltaMSG = "FAULT: Zero Cross Delta over limit\n";
const char* overvoltageMSG = "FAULT: Supply Overvoltage\n";
const char* undervoltageMSG = "FAULT: Supply Undervoltage\n";
const char* onboardFaultMSG = "FAULT: On-board fault\n";

const char* updiResetMSG = "INFO: UPDI Reset\n";
const char* swResetMSG = "INFO: Software Reset\n";
const char* wdResetMSG = "INFO: Watchdog Reset\n";
const char* extResetMSG = "INFO: External Reset\n";
const char* borResetMSG = "INFO: Brown-Out Reset\n";
const char* porResetMSG = "INFO: Power-On Reset\n";
const char* unknownResetMSG = "INFO: Unknown Reset Cause\n";


const char* infoControlModeSetMSG = "INFO: Control Mode set to: ";
const char* infoControlModeRampUpMSG = "Ramp Up\n";
const char* infoControlModeOpenLoopMSG = "Open Loop\n";
const char* infoControlModeClosedLoopMSG = "Closed Loop\n";
const char* infoControlModeStopMSG = "Stop\n";
const char* infoControlModeFaultMSG = "Fault\n";
const char* infoControlModeInit = "Init\n";

static uint32_t motorRPM = 0;
static int16_t motorCurrent = 0;
static uint16_t systemVoltage = 0;
static uint8_t reset_reg = 0;

static controlMode_t systemControlMode, prev_systemControlMode;

static volatile uint8_t mainLoopFlag = 0;
static volatile uint8_t debounce_ongoing = 0;
static volatile uint8_t debounce_counter = 0;

static const uint8_t start_token = 0x03;
static const uint8_t stop_token = 0xFC;

static void app_faultHandler(void);
static void app_printResetCause(void);
static void app_taskHandler(void);
static void app_checkControlMode(void);

static void app_periodic_ISR(void);
static void app_BTN2_ISR();
static void app_BTN1_ISR();

void APP_init(void)
{
    app_printResetCause();
    prev_systemControlMode = MOTOR_STOP_MODE;
    APP_setControlMode(MOTOR_INIT);
    HAL_setPeriodicAppIntCb(&app_periodic_ISR);
    HAL_setBTN1Cb(&app_BTN1_ISR);
    HAL_setBTN2Cb(&app_BTN2_ISR);
    MOTOR_setDirection(DIRECTION_CLOCKWISE);
    MOTOR_stop();

}

void APP_run(void)
{
    /* Check if the flag is set */
    if (mainLoopFlag)
    {
        app_checkControlMode();
        app_taskHandler();
        mainLoopFlag = 0;
        if (debounce_ongoing)
        {
            debounce_counter += 5;
            if (debounce_counter >= BTN_DEBOUNCE_DELAY)
            {
                debounce_ongoing = 0;
                debounce_counter = 0;
                HAL_enableButtonsINT();
            }
        }
    }
}

void APP_setControlMode(controlMode_t setControlMode)
{
    /* Set the new control mode */
    systemControlMode = setControlMode;
}

static void app_checkControlMode(void)
{
    if (systemControlMode != prev_systemControlMode)
    {
        /* Send the new control mode via UART */
        COMM_debugPrint(infoControlModeSetMSG);
        switch (systemControlMode)
        {
            case MOTOR_RAMP_UP_MODE:
                COMM_debugPrint(infoControlModeRampUpMSG);
                break;

            case MOTOR_OPEN_LOOP_MODE:
                COMM_debugPrint(infoControlModeOpenLoopMSG);
                break;

            case MOTOR_CLOSED_LOOP_MODE:
                COMM_debugPrint(infoControlModeClosedLoopMSG);
                break;

            case MOTOR_STOP_MODE:
                COMM_debugPrint(infoControlModeStopMSG);
                break;

            case FAULT_MODE:
                COMM_debugPrint(infoControlModeFaultMSG);
                break;

            case MOTOR_INIT:
                COMM_debugPrint(infoControlModeInit);
                break;

            default:
                break;
        }
        prev_systemControlMode = systemControlMode;
    }
}

void APP_getResetCause(void)
{
    reset_reg = RSTCTRL.RSTFR;
    RSTCTRL.RSTFR = 0xFF;
}

/* Main control task called from the main loop */
static void app_taskHandler(void)
{
    /* Check the control mode and call the corresponding routine handler */
    switch (systemControlMode)
    {
        case MOTOR_RAMP_UP_MODE:
            systemVoltage = HAL_getSupplyVoltage();
            MOTOR_feedSupplyVoltage(systemVoltage);
            motorCurrent = HAL_getMotorCurrent();
            motorRPM = MOTOR_getRPM();

#ifdef ENABLE_DATA_VISUALIZER            
            putc(start_token, stdout);
            fwrite(&motorRPM, 1, sizeof (motorRPM), stdout);
            fwrite(&motorCurrent, 1, sizeof (motorCurrent), stdout);
            fwrite(&systemVoltage, 1, sizeof (systemVoltage), stdout);
            putc(stop_token, stdout);
#endif

            break;

        case MOTOR_OPEN_LOOP_MODE:
            MOTOR_setDuty(HAL_getPotValue() >> (HAL_ADC_RESOLUTION_BITS - 8)); //shift right to map the value to 8 bits
            systemVoltage = HAL_getSupplyVoltage();
            motorRPM = MOTOR_getRPM();
            MOTOR_feedSupplyVoltage(systemVoltage);
            motorCurrent = HAL_getMotorCurrent();

#ifdef ENABLE_DATA_VISUALIZER             
            putc(start_token, stdout);
            fwrite(&motorRPM, 1, sizeof (motorRPM), stdout);
            fwrite(&motorCurrent, 1, sizeof (motorCurrent), stdout);
            fwrite(&systemVoltage, 1, sizeof (systemVoltage), stdout);
            putc(stop_token, stdout);
#endif

            break;

        case MOTOR_CLOSED_LOOP_MODE:
            MOTOR_setDuty(HAL_getPotValue() >> (HAL_ADC_RESOLUTION_BITS - 8)); //shift right to map the value to 8 bits
            systemVoltage = HAL_getSupplyVoltage();
            motorRPM = MOTOR_getRPM();
            MOTOR_feedSupplyVoltage(systemVoltage);
            motorCurrent = HAL_getMotorCurrent();

#ifdef ENABLE_DATA_VISUALIZER            
            putc(start_token, stdout);
            fwrite(&motorRPM, 1, sizeof (motorRPM), stdout);
            fwrite(&motorCurrent, 1, sizeof (motorCurrent), stdout);
            fwrite(&systemVoltage, 1, sizeof (systemVoltage), stdout);
            putc(stop_token, stdout);
#endif

            break;

        case MOTOR_STOP_MODE:
            systemVoltage = HAL_getSupplyVoltage();
            MOTOR_feedSupplyVoltage(systemVoltage);
            motorCurrent = HAL_getMotorCurrent();

#ifdef ENABLE_DATA_VISUALIZER            
            putc(start_token, stdout);
            for (uint8_t i = 0; i < (sizeof (motorRPM)); i++)
            {/*No current and RPM in stop mode*/
                putc(0, stdout);
            }
            fwrite(&motorCurrent, 1, sizeof (motorCurrent), stdout);
            fwrite(&systemVoltage, 1, sizeof (systemVoltage), stdout);
            putc(stop_token, stdout);
#endif

            break;

        case FAULT_MODE:
            app_faultHandler();
            systemVoltage = HAL_getSupplyVoltage();
            /*Already in fault mode, don't need to trigger another fault*/

#ifdef ENABLE_DATA_VISUALIZER             
            /*Send to Data Visualizer*/
            putc(start_token, stdout);
            for (uint8_t i = 0; i < (sizeof (motorRPM) + sizeof (motorCurrent)); i++)
            {/*No current and RPM in fault mode*/
                putc(0, stdout);
            }
            fwrite(&systemVoltage, 1, sizeof (systemVoltage), stdout);
            putc(stop_token, stdout);
#endif

            break;

        case MOTOR_INIT:
            break;

        default:
            break;
    }
}

static void app_faultHandler(void)
{
    systemFault_t faultCode;
    faultCode = MOTOR_getFaultCode();
    if (faultCode != FAULT_CLEAR)
    {
        /* Check for the fault code and display an fault message */
        switch (faultCode)
        {
            case FAULT_OVERCURRENT:
                COMM_debugPrint(overcurrentMSG);
                break;

            case FAULT_ZERO_CROSS_DELTA_OVER_LIMIT:
                COMM_debugPrint(zeroCrossDeltaMSG);
                break;

            case FAULT_ZERO_CROSS_TIMEOUT:
                COMM_debugPrint(zeroCrossTimeoutMSG);
                break;

            case FAULT_OVERVOLTAGE:
                COMM_debugPrint(overvoltageMSG);
                break;

            case FAULT_UNDERVOLTAGE:
                COMM_debugPrint(undervoltageMSG);
                break;

            case FAULT_ONBOARD_FAULT:
                COMM_debugPrint(onboardFaultMSG);
                break;

            default:
                break;
        }
        MOTOR_clearFaultCode();
    }
}

static void app_printResetCause(void)
{
    switch (reset_reg)
    {
        case RSTCTRL_UPDIRF_bm:
            COMM_debugPrint(updiResetMSG);
            break;

        case RSTCTRL_SWRF_bm:
            COMM_debugPrint(swResetMSG);
            break;

        case RSTCTRL_WDRF_bm:
            COMM_debugPrint(wdResetMSG);
            break;

        case RSTCTRL_EXTRF_bm:
            COMM_debugPrint(extResetMSG);
            break;

        case RSTCTRL_BORF_bm:
            COMM_debugPrint(borResetMSG);
            break;

        case RSTCTRL_PORF_bm:
            COMM_debugPrint(porResetMSG);
            break;

        default:
            COMM_debugPrint(unknownResetMSG);
            break;
    }
}

void app_BTN1_ISR()
{
    /*Start/Stop button pressed*/
    HAL_disableButtonsINT();
    debounce_ongoing = 1;
    if (systemControlMode == FAULT_MODE)
    {
        MOTOR_stop();
    }
    else
    {
        if (systemControlMode == MOTOR_RAMP_UP_MODE ||
                systemControlMode == MOTOR_OPEN_LOOP_MODE ||
                systemControlMode == MOTOR_CLOSED_LOOP_MODE)
        {
            MOTOR_stop();
        }
        else
        {
            MOTOR_start();
        }
    }
}

void app_BTN2_ISR(void)
{
    HAL_disableButtonsINT();
    debounce_ongoing = 1;
    if (systemControlMode == MOTOR_STOP_MODE)
    {

        if (MOTOR_getDirection() == DIRECTION_CLOCKWISE)
        {
            MOTOR_setDirection(DIRECTION_ANTICLOCKWISE);
        }
        else
        {
            MOTOR_setDirection(DIRECTION_CLOCKWISE);
        }
    }
}

void app_periodic_ISR(void)
{
    mainLoopFlag = 1;
}