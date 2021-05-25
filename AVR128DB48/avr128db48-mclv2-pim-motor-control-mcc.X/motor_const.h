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

#ifndef MOTOR_CONST_H
#define	MOTOR_CONST_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include "motor_control_hal.h"     
#include "mcc_generated_files/config/clock_config.h"      

  /*Miscellaneous defs - DO NOT CHANGE*/
  /* Number of trapezoidal steps */
#define NUMBER_OF_STEPS                     6

  /* RPM constant used to compute the RPM */
#define RPM_CONSTANT                        60

#define motor_getBlankingTime(trapezoidal_step_half_time) (trapezoidal_step_half_time >> 1)

#define MOTOR_STARTUP_MULTIPLIER           (((F_CPU)/((HAL_OPEN_LOOP_TIMER_PRESCALER) * 10000))-1)

#define ONE_MILISECOND_OPEN_LOOP_TIMER_VAL (((F_CPU)/((HAL_OPEN_LOOP_TIMER_PRESCALER) * 1000))-1)       

  /*Ramp-up defs*/
  typedef enum
  {
    RAMPUP_ALIGN,
    RAMPUP_ACCELERATE,
    RAMPUP_SUSTAIN,
    RAMPUP_BLANKING,
    RAMPUP_CLOSEDLOOP,
    RAMPUP_OPENLOOP
  } rampUpState_t;


  //8 bit pwm input value
#define MAX_PWM_DUTY 255
#define MIN_PWM_DUTY 0 

#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_CONST_H */