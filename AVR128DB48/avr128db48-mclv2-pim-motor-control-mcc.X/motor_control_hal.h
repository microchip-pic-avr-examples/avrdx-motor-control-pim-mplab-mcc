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

#ifndef MOTOR_CONTROL_HAL_H
#define	MOTOR_CONTROL_HAL_H

#include "mcc_generated_files/mcc.h"
#include "board_params.h"
#include <stdint.h>

#ifdef	__cplusplus
extern "C"
{
#endif

  /*Callbacks*/
#define HAL_setOvercurrentCb(x)     AC0_SetIsrCallback(x)
#define HAL_setZeroCrossCb(x)       AC1_SetIsrCallback(x)
#define HAL_setBEMFblankingCb(x)    TCA1_SetCMP0IsrCallback(x)
#define HAL_setBEMFtimingCb(x)      TCA1_SetCMP1IsrCallback(x)
#define HAL_setBEMFtimeoutCb(x)     TCA1_SetCMP2IsrCallback(x)
#define HAL_setOpenLoopTimerCb(x)   TCA0_SetCMP0IsrCallback(x)
#define HAL_setOnboardFaultCb(x)    PORTC_BUS_CURRENT_FLT_SetInterruptHandler(x)    
#define HAL_setBTN1Cb(x)            PORTF_BTN_1_SetInterruptHandler(x)
#define HAL_setBTN2Cb(x)            PORTF_BTN_2_SetInterruptHandler(x)     
#define HAL_setPeriodicAppIntCb(x)  TCB0_SetCaptIsrCallback(x)       

#define HAL_disableButtonsINT()         do{BTN_1_DisableInterruptOnChange(); BTN_2_DisableInterruptOnChange();}while(0)  
#define HAL_enableButtonsINT()          do{BTN_1_EnableInterruptForFallingEdge(); BTN_2_EnableInterruptForFallingEdge();}while(0) 


#define HAL_enableOvercurrentINT()      do{ AC0.STATUS |= AC_CMPIF_bm; AC0.INTCTRL |= AC_CMP_bm; }while(0)
#define HAL_disableOvercurrentINT()     (AC0.INTCTRL &= ~AC_CMP_bm)

#define HAL_enableOnBoardFaultINT()     BUS_CURRENT_FLT_EnableInterruptForRisingEdge()
#define HAL_disableOnBoardFaultINT()    BUS_CURRENT_FLT_DisableInterruptOnChange()   

#define HAL_disableZeroCrossINT()       (AC1.INTCTRL &= ~AC_CMP_bm)
#define HAL_enableZeroCrossINT()        do{ AC1.STATUS = AC_CMPIF_bm; AC1.INTCTRL |= AC_CMP_bm; }while(0)


  /*LEDs*/
#define HAL_clockwiseLEDturnOn()        DBG_LED1_SetHigh()
#define HAL_clockwiseLEDturnOff()       DBG_LED1_SetLow()
#define HAL_anticlockwiseLEDturnOn()    DBG_LED2_SetHigh()
#define HAL_anticlockwiseLEDturnOff()   DBG_LED2_SetLow()    


  /*Must be set in accordance to the MCC set values*/
#define HAL_CLOSED_LOOP_TIMER_PRESCALER     4
#define HAL_OPEN_LOOP_TIMER_PRESCALER       256UL


  /*Closed loop timer*/
#define HAL_closedLoopTimerGetCounter()     (TCA1.SINGLE.CNT)

#define HAL_setBEMFblankingTime(x)          (TCA1.SINGLE.CMP0 = (x))
#define HAL_setNextSectorTime(x)            (TCA1.SINGLE.CMP1 = (x))
#define HAL_setBEMFtimeout(x)               (TCA1.SINGLE.CMP2 = (x))

#define HAL_disableBEMFblankingINT()        (TCA1.SINGLE.INTCTRL &= ~TCA_SINGLE_CMP0_bm)
#define HAL_disableNextSectorINT()          (TCA1.SINGLE.INTCTRL &= ~TCA_SINGLE_CMP1_bm)
#define HAL_disableBEMFtimeoutINT()         (TCA1.SINGLE.INTCTRL &= ~TCA_SINGLE_CMP2_bm)

#define HAL_enableBEMFblankingINT()         (TCA1.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm)
#define HAL_enableNextSectorINT()           (TCA1.SINGLE.INTCTRL |= TCA_SINGLE_CMP1_bm)
#define HAL_enableBEMFtimeoutINT()          (TCA1.SINGLE.INTCTRL |= TCA_SINGLE_CMP2_bm)

#define HAL_closedLoopTimerEnable()          do{TCA1.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm | TCA_SINGLE_CMP1_bm | TCA_SINGLE_CMP2_bm;\
                                                TCA1.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;}while(0)
#define HAL_closedLoopTimerDisable()         do{TCA1.SINGLE.INTFLAGS = 0xFF; TCA1.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;}while(0)
#define HAL_closedLoopTimerReset()           (TCA1.SINGLE.CNT = 0)

  /* Minimum TCA CMP1 value for BEMF timing */
#define HAL_MIN_BEMF_TIMER_VALUE              2


  /*Open loop timer*/
#define HAL_openLoopTimerEnable()            do{TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm | TCA_SINGLE_CMP1_bm | TCA_SINGLE_CMP2_bm;\
                                                TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;}while(0)
#define HAL_openLoopTimerDisable()          (TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm)
#define HAL_openLoopTimerReset()            (TCA0.SINGLE.CNT= 0)
#define HAL_setOpenLoopStepDuration(x)      (TCA0.SINGLE.CMP0 = (x))
#define HAL_enableOpenLoopINT()             (TCA0.SINGLE.INTCTRL  |= TCA_SINGLE_CMP0_bm)

  /*PWM timer defs*/
#define HAL_enablePWM()                     (TCD0.CTRLA |= TCD_ENABLE_bm)
#define HAL_disablePWM()                    do{TCD0.CTRLA &= ~TCD_ENABLE_bm; TCD0.CTRLC &= ~(TCD_CMPDSEL_bm | TCD_CMPCSEL_bm); }while(0)   


  /*Analog part references*/
#define ADC_VREF VDD_VREF
#define AC_VREF  VDD_VREF  

#define HAL_ADC_RESOLUTION_BITS 12

#define AC_DACREF_RESOLUTION_BITS 8
#define AC_DACREF_MAX_VALUE                 ((uint16_t)(1<< AC_DACREF_RESOLUTION_BITS) -1)    

  /* Motor current computation*/
#define HAL_overcurrentCompute(I)           ((((int32_t)I*(int32_t)MOTOR_CURRENT_SHUNT_SENS)/(int32_t)MOTOR_CURRENT_MULTIPLIER_MA +\
                                            (int32_t)MOTOR_CURRENT_SHUNT_VOLTAGE_OFFSET)*(int32_t)AC_DACREF_MAX_VALUE/(int32_t)AC_VREF)

#define HAL_setOvercurrentUpperRef(x)       (AC0.DACREF = (uint8_t)HAL_overcurrentCompute(x))
#define HAL_setOvercurrentLowerRef(x)       (AC2.DACREF = (uint8_t)HAL_overcurrentCompute(x)) 


  /*BEMF part*/
#define HAL_BEMF_RISING     AC_INTMODE_NORMAL_POSEDGE_gc
#define HAL_BEMF_FALLING    AC_INTMODE_NORMAL_NEGEDGE_gc  

#define HAL_BEMFselectPhaseA(x) do { AC1.MUXCTRL = AC_MUXPOS_AINP1_gc | AC_MUXNEG_AINN0_gc; AC1.INTCTRL = x;}while(0)
#define HAL_BEMFselectPhaseB(x) do { AC1.MUXCTRL = AC_MUXPOS_AINP3_gc | AC_MUXNEG_AINN0_gc; AC1.INTCTRL = x;}while(0)
#define HAL_BEMFselectPhaseC(x) do { AC1.MUXCTRL = AC_MUXPOS_AINP2_gc | AC_MUXNEG_AINN0_gc; AC1.INTCTRL = x;}while(0)



  /*Public functions*/
  void HAL_PWMsetDuty (uint16_t dutyCycle);
  uint16_t HAL_getSupplyVoltage (void);
  int16_t HAL_getMotorCurrent (void);
  uint16_t HAL_getPotValue (void);

  void HAL_driveAH_BL (void);
  void HAL_driveAH_CL (void);
  void HAL_driveBH_CL (void);
  void HAL_driveBH_AL (void);
  void HAL_driveCH_AL (void);
  void HAL_driveCH_BL (void);
  void HAL_driveLowside (void);


#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_CONTROL_HAL_H */

