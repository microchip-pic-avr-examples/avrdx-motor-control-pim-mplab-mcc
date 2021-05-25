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

#include "motor_control.h"
#include "motor_control_hal.h"
#include "motor_control_params.h"
#include "motor_const.h"

/* Local variables */
static volatile systemFault_t faultCode;
static volatile motorDirection_t motor_dir = DIRECTION_CLOCKWISE;
static volatile rampUpState_t motorRampUp_state;

static volatile int32_t current_zero_cross_time;
static volatile uint32_t t_current;

static volatile uint16_t commutation_counter;
static volatile uint16_t previous_zero_cross_time;
static volatile uint16_t t_sector;
static volatile uint16_t prev_mapped_duty_cycle;
static volatile uint16_t duty_counter;

static volatile uint8_t sequence_step;
static volatile uint8_t faultPresent;
static volatile uint8_t motorRampUpBlankingSteps;

/*Private functions*/
static void motor_faultProtection(uint8_t fault);
static void motor_brake(uint16_t duty);
static void motor_triggerStep(void);
static void motor_commutationTiming(void);
static void motor_setNextStep(void);

/*Motor Control Interrupt Service Routines*/
static void motor_Overcurrent_ISR();
static void motor_onBoardFault_ISR();
static void motor_BEMF_zero_cross_ISR();
static void motor_BEMF_blanking_ISR();
static void motor_timer_next_sector_ISR();
static void motor_BEMF_timeout_ISR();
static void motor_ramp_up_ISR();

void MOTOR_init(void)
{
    HAL_setOvercurrentCb(&motor_Overcurrent_ISR);
    HAL_setZeroCrossCb(&motor_BEMF_zero_cross_ISR);
    HAL_setBEMFblankingCb(&motor_BEMF_blanking_ISR);
    HAL_setBEMFtimingCb(&motor_timer_next_sector_ISR);
    HAL_setBEMFtimeoutCb(&motor_BEMF_timeout_ISR);
    HAL_setOpenLoopTimerCb(&motor_ramp_up_ISR);
    HAL_setOnboardFaultCb(&motor_onBoardFault_ISR);

    /* Set Current window values */
    HAL_setOvercurrentUpperRef(MOTOR_CURRENT_UPPER_LIMIT);
    HAL_setOvercurrentLowerRef(MOTOR_CURRENT_LOWER_LIMIT);

    HAL_setBEMFtimeout(MOTOR_ZERO_CROSS_TIMEOUT_VALUE);
    HAL_enableBEMFtimeoutINT();

    HAL_disableOnBoardFaultINT();

    /*By default, current window interrupts are enabled all the time but may cause
     problems at ramp-up with motors that draw too much current. 
     Disable the overcurrent interrupts here and enable them at the end of the ramp-up
     sequence, at your own risk.*/
    HAL_enableOvercurrentINT();
    //HAL_disableOvercurrentINT();
    /* Disable BEMF interrupts */
    HAL_disableZeroCrossINT();

    /* Disable closed loop timer and interrupts */
    HAL_closedLoopTimerDisable();
    HAL_closedLoopTimerReset();
    HAL_disableBEMFblankingINT();
    HAL_disableNextSectorINT();

    /* Disable TCA0 Timer */
    HAL_openLoopTimerDisable();
    HAL_openLoopTimerReset();

    commutation_counter = 0;
    sequence_step = 0;
    motorRampUp_state = RAMPUP_ALIGN;
    current_zero_cross_time = 0;
    previous_zero_cross_time = 0;
    t_current = MOTOR_STARTUP_INITIAL_STEP_PERIOD * 10;
    t_sector = 1;
    prev_mapped_duty_cycle = MOTOR_MIN_PWM_DUTY;
    faultCode = FAULT_CLEAR;
    faultPresent = 0;
    duty_counter = 0;
    motorRampUpBlankingSteps = 0;
}

void MOTOR_setDirection(motorDirection_t dir)
{
    motor_dir = dir;
    if (dir == DIRECTION_CLOCKWISE)
    {
        HAL_clockwiseLEDturnOn();
        HAL_anticlockwiseLEDturnOff();
    }
    else
    {
        HAL_clockwiseLEDturnOff();
        HAL_anticlockwiseLEDturnOn();
    }
}

motorDirection_t MOTOR_getDirection(void)
{
    return motor_dir;
}

void MOTOR_setDuty(uint8_t duty_cycle)
{
    uint16_t temp;
    /*Map the duty cycle to the PWM interval*/

    temp = ((uint32_t)duty_cycle * (uint32_t)(MOTOR_MAX_PWM_DUTY - MOTOR_MIN_PWM_DUTY)) / (uint32_t)(255) + (uint32_t)MOTOR_MIN_PWM_DUTY;

    /*Filter the input signal with a moving average*/
    /*Mainl*/
    temp = (uint32_t)(((uint32_t)prev_mapped_duty_cycle * (32 - 1) + (uint32_t)temp)) / 32;
    prev_mapped_duty_cycle = temp;
    HAL_PWMsetDuty(temp);

}

uint32_t MOTOR_getRPM(void)
{
    uint32_t calculatedRpm;
    calculatedRpm = previous_zero_cross_time;
    if (calculatedRpm != 0)
    {
        calculatedRpm = (RPM_CONSTANT * F_CPU) /
                /* The zero cross timer value is the zero-cross time, which is half the time of a
                trapezoidal step, and we must multiply it with 2 to get the correct value */
                (HAL_CLOSED_LOOP_TIMER_PRESCALER * NUMBER_OF_STEPS * MOTOR_POLE_PAIRS * (calculatedRpm * 2));
    }
    else
    {
        calculatedRpm = 0;
    }
    return calculatedRpm;
}

void MOTOR_feedSupplyVoltage(uint16_t measured_voltage)
{

    if (measured_voltage > SUPPLY_OVP)
    {
        motor_faultProtection(FAULT_OVERVOLTAGE);
    }
    else if (measured_voltage < SUPPLY_UVP)
    {
        motor_faultProtection(FAULT_UNDERVOLTAGE);
    }

}

void MOTOR_feedCurrent(int16_t measured_current)
{
    //do any needed operation with the adc measured current
}

systemFault_t MOTOR_getFaultCode()
{
    return faultCode;
}

void MOTOR_clearFaultCode(void)
{
    faultCode = FAULT_CLEAR;
}

/*Function called from timer ISR*/
/*Handles both the ramp-up sequence and the open loop mode*/
static void motor_openLoopHandler(void)
{

    switch (motorRampUp_state)
    {

        case RAMPUP_ALIGN:


            /* Trigger the step and increase the PWM */
            /* This is done in order to get a smooth alignment, without any mechanical shock */
            if (duty_counter < MOTOR_STARTUP_PWM_DUTY)
            {
                HAL_PWMsetDuty(duty_counter);

                duty_counter++;

            }
                /* Enable ramp-up timer */
            else
            {
                motorRampUp_state = RAMPUP_ACCELERATE;
            }
            HAL_openLoopTimerDisable();
            HAL_openLoopTimerReset();
            HAL_openLoopTimerEnable();
            break;

        case RAMPUP_ACCELERATE:
            if (t_current < (uint32_t)(MOTOR_STARTUP_RAMP_DURATION * 10))
            {
                t_sector = (uint32_t)MOTOR_STARTUP_CONSTANT / t_current;
                //calculates T for 1 sector, in 100s of uS.
                if (t_sector <= 0) t_sector = 1;
                t_current += t_sector;
            }
            else
            {
                motorRampUp_state = RAMPUP_SUSTAIN;
            }
            HAL_openLoopTimerDisable();
            HAL_openLoopTimerReset();
            HAL_setOpenLoopStepDuration(t_sector * MOTOR_STARTUP_MULTIPLIER);
            HAL_openLoopTimerEnable();
            motor_setNextStep();
            motor_triggerStep();
            /*Required to visualize the rpm during ramp-up sequence*/
            previous_zero_cross_time = (t_sector * MOTOR_STARTUP_MULTIPLIER) * (HAL_OPEN_LOOP_TIMER_PRESCALER / (2 * HAL_CLOSED_LOOP_TIMER_PRESCALER));

            break;

        case RAMPUP_SUSTAIN:
            motor_setNextStep();
            motor_triggerStep();
            if (t_current < (uint32_t)((MOTOR_STARTUP_RAMP_DURATION * 10 + MOTOR_STARTUP_SUSTAIN * 10)))
            {
                t_current += t_sector;
            }
            else
            {
                if (MOTOR_CONTROL_MODE == MOTOR_CLOSED_LOOP_MODE)
                {
                    motorRampUp_state = RAMPUP_BLANKING;
                    for (uint8_t i = 0; i < MOTOR_STARTUP_HOLDOFF_STEPS + 2; i++)
                    {
                        motor_setNextStep();
                    }
                }
                else if (MOTOR_CONTROL_MODE == MOTOR_OPEN_LOOP_MODE)
                {
                    APP_setControlMode(MOTOR_OPEN_LOOP_MODE);
                    motorRampUp_state = RAMPUP_OPENLOOP;
                }
            }

            /*Prepare next interrupt*/
            HAL_openLoopTimerDisable();
            HAL_openLoopTimerReset();
            HAL_setOpenLoopStepDuration(t_sector * MOTOR_STARTUP_MULTIPLIER);
            HAL_openLoopTimerEnable();
            break;

        case RAMPUP_BLANKING:
            /*Count the blanking steps*/
            HAL_disablePWM();
            motorRampUpBlankingSteps++;
            if ((motorRampUpBlankingSteps >= MOTOR_STARTUP_HOLDOFF_STEPS))
            {
                motorRampUp_state = RAMPUP_CLOSEDLOOP;
            }

            /*Prepare next interrupt*/
            /*Step duration does not change*/
            HAL_openLoopTimerDisable();
            HAL_openLoopTimerReset();
            HAL_openLoopTimerEnable();
            break;

        case RAMPUP_CLOSEDLOOP:
            /* open loop timer not needed in closed loop */
            HAL_openLoopTimerDisable();
            HAL_openLoopTimerReset();

            /*Set the current and previous zero cross time to half of the last commutation period*/
            current_zero_cross_time = previous_zero_cross_time;

            /* Set zero-cross blanking time */
            HAL_setBEMFblankingTime(HAL_MIN_BEMF_TIMER_VALUE);

            HAL_closedLoopTimerReset();

            /* Enable Zero Cross Blanking Interrupt */
            HAL_enableBEMFblankingINT();
            HAL_closedLoopTimerEnable();

            APP_setControlMode(MOTOR_CLOSED_LOOP_MODE);
            break;

        case RAMPUP_OPENLOOP:
            /*Default step in the open-loop mode after ramp-up*/
            /*Manually switch to the next trapezoidal step*/
            motor_setNextStep();
            motor_triggerStep();

            /*Reset the timer and enable it for the next commutation*/
            HAL_openLoopTimerDisable();
            HAL_openLoopTimerReset();
            HAL_openLoopTimerEnable();
            break;

        default:
            break;
    }

}

static void motor_faultProtection(uint8_t fault)
{

    /* Disable the motor control peripheral and interrupts */
    HAL_disableZeroCrossINT();
    HAL_closedLoopTimerDisable();
    HAL_openLoopTimerDisable();
    HAL_disablePWM();

    /* Set control mode to FAULT and update fault code */
    faultCode = fault;
    faultPresent = 1;
    APP_setControlMode(FAULT_MODE);

}

void MOTOR_start(void)
{

    APP_setControlMode(MOTOR_RAMP_UP_MODE);
    HAL_setOpenLoopStepDuration((uint16_t)(MOTOR_ALIGN_STEP_DURATION));
    HAL_openLoopTimerReset();
    HAL_enableOpenLoopINT();
    HAL_openLoopTimerEnable();
    HAL_PWMsetDuty(0);
    motor_triggerStep();


}

void MOTOR_stop(void)
{

    /* Reset all variables */
    HAL_disablePWM();
    MOTOR_init();
    APP_setControlMode(MOTOR_STOP_MODE);
    motor_brake(MOTOR_BRAKE_DUTY);

}

static void motor_brake(uint16_t duty)
{

    /*Apply non-regenerative braking*/

    HAL_driveLowside();
    HAL_PWMsetDuty(duty);
    HAL_enablePWM();

}

static void motor_commutationTiming(void)
{
    current_zero_cross_time = HAL_closedLoopTimerGetCounter();
    /* Compute zero-cross delta time, based on previous, filtered, zero-cross value */
#if MOTOR_COMMUTATION_TIME_DELTA != 0
    if (abs((int32_t)current_zero_cross_time - (int32_t)previous_zero_cross_time) > (previous_zero_cross_time / MOTOR_COMMUTATION_TIME_DELTA))
    {
        motor_faultProtection(FAULT_ZERO_CROSS_DELTA_OVER_LIMIT);
    }
#endif
    /* Filter the zero-cross time with a moving average */
    current_zero_cross_time = (uint32_t)(((uint32_t)previous_zero_cross_time * (MOTOR_DIVISION_FACTOR - 1) +
            (uint32_t)current_zero_cross_time)) / MOTOR_DIVISION_FACTOR;

    /*Current zero cross time becomes the previous value on the next zero-cross event*/
    previous_zero_cross_time = current_zero_cross_time;

    /* Add the advance angle and compensation */
    current_zero_cross_time = current_zero_cross_time - ((current_zero_cross_time * MOTOR_ADVANCE_ANGLE) / 30) -
            MOTOR_PASSTHROUGH_DELAY_COMP;

    /* Check if the value is less than the minimum BEMF timer value */
    if (current_zero_cross_time < HAL_MIN_BEMF_TIMER_VALUE)
    {
        current_zero_cross_time = HAL_MIN_BEMF_TIMER_VALUE;
    }

    HAL_setNextSectorTime(current_zero_cross_time);
}

/*********************************************************/
/*             INTERRUPT SERVICE ROUTINES                */

/*********************************************************/

static void motor_Overcurrent_ISR()
{
    /* Overcurrent Interrupt */
    motor_faultProtection(FAULT_OVERCURRENT);
}

static void motor_onBoardFault_ISR()
{
    /* On-board Overcurrent Interrupt */
    motor_faultProtection(FAULT_ONBOARD_FAULT);
}

/*The following ISRs have the exact order of execution*/
static void motor_BEMF_blanking_ISR()
{
    /* BEMF Blanking Interupt */
    HAL_enableZeroCrossINT();
    HAL_disableBEMFblankingINT();
}

/*Zero-cross ISR of the BEMF*/
static void motor_BEMF_zero_cross_ISR()
{
    HAL_closedLoopTimerDisable();
    motor_commutationTiming();
    HAL_closedLoopTimerReset();


    HAL_enableNextSectorINT();
    HAL_disableZeroCrossINT();

    /*To check if any fault occured so the timer will not start again
     as it was stopped in the fault protection routine*/
    if (!faultPresent)
    {
        HAL_closedLoopTimerEnable();
    }
}

/*Commutation time done, trigger next step*/
static void motor_timer_next_sector_ISR()
{
    motor_setNextStep();
    /* Apply the new trapezoidal step */
    motor_triggerStep();
    HAL_closedLoopTimerDisable();
    HAL_closedLoopTimerReset();
    HAL_closedLoopTimerEnable();
    /* Set new zero-cross blanking time and enable the interrupt */
    HAL_setBEMFblankingTime(motor_getBlankingTime(previous_zero_cross_time));

    HAL_enableBEMFblankingINT();
    HAL_disableNextSectorINT();
}
/*Back to BEMF blanking ISR*/

/*Called when the RPM is too low or when the zero cross is not detected*/
static void motor_BEMF_timeout_ISR()
{
    motor_faultProtection(FAULT_ZERO_CROSS_TIMEOUT);
}

static void motor_ramp_up_ISR()
{
    /* Ramp-up Interrupt */
    motor_openLoopHandler();
}

/*Six step state machine*/
static void motor_triggerStep(void)
{
    switch (sequence_step)
    {
        case 0:
            HAL_driveAH_BL();
            break;
        case 1:
            HAL_driveAH_CL();
            break;
        case 2:
            HAL_driveBH_CL();
            break;
        case 3:
            HAL_driveBH_AL();
            break;
        case 4:
            HAL_driveCH_AL();
            break;
        case 5:
            HAL_driveCH_BL();
            break;
        default:
            break;
    }
}

static void motor_setNextStep(void)
{
    /* Move to the next trapezoidal step */
    if (motor_dir == DIRECTION_CLOCKWISE)
    {
        sequence_step++;
        if (sequence_step >= NUMBER_OF_STEPS)
        {
            /* Move back to 0 if the previous step was 5 */
            sequence_step = 0;
        }
        switch (sequence_step)
        {
            case 0:
                HAL_BEMFselectPhaseC(HAL_BEMF_FALLING);
                /* Phase C floating - Falling */
                break;

            case 1:
                HAL_BEMFselectPhaseB(HAL_BEMF_RISING);
                /* Phase B floating - Rising */
                break;

            case 2:
                HAL_BEMFselectPhaseA(HAL_BEMF_FALLING);
                /* Phase A floating - Falling */
                break;

            case 3:
                HAL_BEMFselectPhaseC(HAL_BEMF_RISING);
                /* Phase C floating - Rising */
                break;

            case 4:
                HAL_BEMFselectPhaseB(HAL_BEMF_FALLING);
                /* Phase B floating - Falling */
                break;

            case 5:
                HAL_BEMFselectPhaseA(HAL_BEMF_RISING);
                /* Phase A floating - Rising */
                break;

            default:
                break;
        }
    }
    else
    {
        if (sequence_step <= 0)
        {
            /* Move back to NUMBER_OF_STEPS if the previous step was 0 */
            sequence_step = NUMBER_OF_STEPS;
        }
        sequence_step--;
        switch (sequence_step)
        {
            case 0:
                HAL_BEMFselectPhaseC(HAL_BEMF_RISING);
                /* Phase C floating - Rising */
                break;

            case 1:
                HAL_BEMFselectPhaseB(HAL_BEMF_FALLING);
                /* Phase B floating - Falling */
                break;

            case 2:
                HAL_BEMFselectPhaseA(HAL_BEMF_RISING);
                /* Phase A floating - Rising */
                break;

            case 3:
                HAL_BEMFselectPhaseC(HAL_BEMF_FALLING);
                /* Phase C floating - Falling */
                break;

            case 4:
                HAL_BEMFselectPhaseB(HAL_BEMF_RISING);
                /* Phase B floating - Rising */
                break;

            case 5:
                HAL_BEMFselectPhaseA(HAL_BEMF_FALLING);
                /* Phase A floating - Falling */
                break;

            default:
                break;
        }
    }
}