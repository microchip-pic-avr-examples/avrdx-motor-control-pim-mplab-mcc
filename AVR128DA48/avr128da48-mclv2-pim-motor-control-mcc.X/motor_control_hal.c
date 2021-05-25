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

#define CCL_TRUTH_TCD_WOA   0x02
#define CCL_TRUTH_TCD_WOB   0x04
#define CCL_DISABLE_OUTPUT  0x00

#define ADC_POT_POSITION        ADC_MUXPOS_AIN0_gc
#define ADC_CURRENT_POSITION    ADC_MUXPOS_AIN2_gc
#define ADC_VBUS_POSITION       ADC_MUXPOS_AIN1_gc
#define ADC_ACC_REG_VALUE       ADC_SAMPNUM_ACC2_gc

#define ADC_MAX_VALUE ((uint16_t)((1<<HAL_ADC_RESOLUTION_BITS)-1))



#define TCD_MIN_PWM_DUTY      ((TCD_WOB_CLR_VALUE)/2)                
#define TCD_MAX_PWM_DUTY      (TCD_WOB_CLR_VALUE - 2*(TCD_DEADTIME)-1)


/* The following macro defines the maximum duty cycle of the system and the PWM frequency */
#define TCD_WOB_CLR_VALUE 512
#define TCD_WOA_SET_VALUE TCD_DEADTIME
#define TCD_WOA_CLR_VALUE 7
#define TCD_WOB_SET_VALUE 7



static void CCL_selectInput(uint8_t LUT0_truth,
        uint8_t LUT1_truth,
        uint8_t LUT4_truth,
        uint8_t LUT3_truth);

static void TCD0_setWaveformOutputs(uint8_t ctrlcMask, uint8_t faultctrlMask);

static void CCL_selectInput(uint8_t LUT0_truth,
        uint8_t LUT1_truth,
        uint8_t LUT4_truth,
        uint8_t LUT3_truth)
{
    /* Disable the CCL peripheral */
    CCL.CTRLA &= ~CCL_ENABLE_bm;

    /* Disable each LUT */
    CCL.LUT0CTRLA &= ~CCL_ENABLE_bm;
    CCL.LUT1CTRLA &= ~CCL_ENABLE_bm;
    CCL.LUT4CTRLA &= ~CCL_ENABLE_bm;
    CCL.LUT3CTRLA &= ~CCL_ENABLE_bm;

    /* Write the new truth value */
    CCL.TRUTH0 = LUT0_truth;
    CCL.TRUTH1 = LUT1_truth;
    CCL.TRUTH4 = LUT4_truth;
    CCL.TRUTH3 = LUT3_truth;

    /* Enable each LUT */
    CCL.LUT0CTRLA |= CCL_ENABLE_bm;
    CCL.LUT1CTRLA |= CCL_ENABLE_bm;
    CCL.LUT4CTRLA |= CCL_ENABLE_bm;
    CCL.LUT3CTRLA |= CCL_ENABLE_bm;

    /* Enable the CCL peripheral */
    CCL.CTRLA |= CCL_ENABLE_bm;
}

void HAL_PWMsetDuty(uint16_t dutyCycle)
{
    uint16_t temp;
    temp = ((uint32_t)dutyCycle * (uint32_t)(TCD_MAX_PWM_DUTY - TCD_MIN_PWM_DUTY)) / (uint32_t)(255) + (uint32_t)TCD_MIN_PWM_DUTY;
    /* Set the new compare values */
    TCD0.CMPACLR = temp + TCD_DEADTIME;
    TCD0.CMPBSET = temp + (2 * TCD_DEADTIME);
    TCD0.CMPBCLR = TCD_WOB_CLR_VALUE;
    TCD0.CMPASET = TCD_DEADTIME;
    TCD0.CTRLE = TCD_SYNCEOC_bm;
}

static void TCD0_setWaveformOutputs(uint8_t ctrlcMask, uint8_t faultctrlMask)
{
    TCD0.CTRLC &= ~(TCD_CMPDSEL_bm | TCD_CMPCSEL_bm);
    TCD0.CTRLC |= ctrlcMask;
    CPU_CCP = CCP_IOREG_gc;

    /* Enable WOC/WOD or disable outputs */
    TCD0.FAULTCTRL = faultctrlMask;
}

uint16_t HAL_getSupplyVoltage(void)
{
    uint32_t adc_measurement;
    adc_measurement = ADC0_GetConversion(ADC_VBUS_POSITION) >> ADC_ACC_REG_VALUE;
    adc_measurement = (adc_measurement * ADC_VREF * SUPPLY_VOLTAGE_DIVIDER_RATIO) / ADC_MAX_VALUE;
    return (uint16_t)adc_measurement;
}

uint16_t HAL_getPotValue(void)
{
    return ADC0_GetConversion(ADC_POT_POSITION)>>(ADC_ACC_REG_VALUE);
}

int16_t HAL_getMotorCurrent(void)
{
    int32_t ADCTemp;

    ADCTemp = ADC0_GetConversion(ADC_CURRENT_POSITION) >> ADC_ACC_REG_VALUE;
    /* Compute the ADC value to mA using the config values */
    ADCTemp = (((int32_t)ADCTemp * ADC_VREF) / (int32_t)ADC_MAX_VALUE) - MOTOR_CURRENT_SHUNT_VOLTAGE_OFFSET;

    return (ADCTemp * MOTOR_CURRENT_MULTIPLIER_MA) / MOTOR_CURRENT_SHUNT_SENS;
}

void HAL_driveAH_BL(void)
{
    HAL_disablePWM();

    CCL_selectInput(CCL_TRUTH_TCD_WOA, CCL_TRUTH_TCD_WOB, CCL_TRUTH_TCD_WOB, CCL_TRUTH_TCD_WOA);
    TCD0_setWaveformOutputs(0, 0);

    HAL_enablePWM();
}

void HAL_driveAH_CL(void)
{
    HAL_disablePWM();

    CCL_selectInput(CCL_TRUTH_TCD_WOA, CCL_TRUTH_TCD_WOB, CCL_DISABLE_OUTPUT, CCL_DISABLE_OUTPUT);
    TCD0_setWaveformOutputs(TCD_CMPCSEL_bm, TCD_CMPDEN_bm | TCD_CMPCEN_bm);

    HAL_enablePWM();
}

void HAL_driveBH_CL()
{
    HAL_disablePWM();

    CCL_selectInput(CCL_DISABLE_OUTPUT, CCL_DISABLE_OUTPUT, CCL_TRUTH_TCD_WOA, CCL_TRUTH_TCD_WOB);
    TCD0_setWaveformOutputs(TCD_CMPCSEL_bm, TCD_CMPDEN_bm | TCD_CMPCEN_bm);

    HAL_enablePWM();
}

void HAL_driveBH_AL()
{
    HAL_disablePWM();

    CCL_selectInput(CCL_TRUTH_TCD_WOB, CCL_TRUTH_TCD_WOA, CCL_TRUTH_TCD_WOA, CCL_TRUTH_TCD_WOB);
    TCD0_setWaveformOutputs(0, 0);

    HAL_enablePWM();
}

void HAL_driveCH_AL(void)
{
    HAL_disablePWM();

    CCL_selectInput(CCL_TRUTH_TCD_WOB, CCL_TRUTH_TCD_WOA, CCL_DISABLE_OUTPUT, CCL_DISABLE_OUTPUT);
    TCD0_setWaveformOutputs(TCD_CMPDSEL_bm, TCD_CMPDEN_bm | TCD_CMPCEN_bm);

    HAL_enablePWM();
}

void HAL_driveCH_BL(void)
{
    HAL_disablePWM();

    CCL_selectInput(CCL_DISABLE_OUTPUT, CCL_DISABLE_OUTPUT, CCL_TRUTH_TCD_WOB, CCL_TRUTH_TCD_WOA);
    TCD0_setWaveformOutputs(TCD_CMPDSEL_bm, TCD_CMPDEN_bm | TCD_CMPCEN_bm);

    HAL_enablePWM();
}

void HAL_driveLowside(void)
{
    CCL_selectInput(CCL_DISABLE_OUTPUT, CCL_TRUTH_TCD_WOA, CCL_DISABLE_OUTPUT, CCL_TRUTH_TCD_WOA);
    TCD0_setWaveformOutputs(0, TCD_CMPDEN_bm);
}