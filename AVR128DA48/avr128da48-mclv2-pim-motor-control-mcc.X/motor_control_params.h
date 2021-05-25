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

#ifndef MOTOR_CONTROL_PARAMS_H
#define	MOTOR_CONTROL_PARAMS_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include "app.h"
#include "motor_const.h"

  /* Control mode after ramp-up */
#define MOTOR_CONTROL_MODE MOTOR_CLOSED_LOOP_MODE
  

  /*Non-regenerative braking duty cycle*/
#define MOTOR_BRAKE_DUTY                    229


  /* Delay in miliseconds between duty cycle increments */
#define MOTOR_ALIGN_STEP_DURATION           368UL


#define MOTOR_MIN_PWM_DUTY                  51
#define MOTOR_MAX_PWM_DUTY                  255


#define MOTOR_STARTUP_PWM_DUTY              63
#define MOTOR_STARTUP_HOLDOFF_STEPS         1


  /* Motor number of pole pairs - Used in RPM computing */
#define MOTOR_POLE_PAIRS                    4


#define MOTOR_STARTUP_INITIAL_STEP_PERIOD   300UL

#define MOTOR_STARTUP_RAMP_DURATION         2000UL
#define MOTOR_STARTUP_TARGET_RPM            800UL
#define MOTOR_STARTUP_CONSTANT              625000UL
#define MOTOR_STARTUP_SUSTAIN               1


  /*Advance angle for the next trapezoidal step*/
#define MOTOR_ADVANCE_ANGLE                 0

  /*Passthrough delay for the zero cross detection*/
#define MOTOR_PASSTHROUGH_DELAY_COMP        1200UL
  /*Filter coefficient used in BEMF zero cross time filtering*/
#define MOTOR_DIVISION_FACTOR               8

  /*Stall and glitch detection parameters*/
#define MOTOR_COMMUTATION_TIME_DELTA        1
  /*Compare value for stall detection */
#define MOTOR_ZERO_CROSS_TIMEOUT_VALUE      26250

  /*Motor overcurrent window protection values*/
#define MOTOR_CURRENT_UPPER_LIMIT           4300UL
#define MOTOR_CURRENT_LOWER_LIMIT           -4300UL


  /*Undervoltage protection in millivolts*/
#define SUPPLY_UVP                          11000
  /*Overvoltage protection in millivolts*/
#define SUPPLY_OVP                          25000

#define MOTOR_MIN_RPM                       MOTOR_STARTUP_TARGET_RPM

#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_CONTROL_PARAMS_H */