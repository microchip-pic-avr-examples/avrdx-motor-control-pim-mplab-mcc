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

#ifndef MOTOR_CONTROL_H
#define	MOTOR_CONTROL_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include "app.h"
#include <stdint.h>

  /* Fault type enum */
  typedef enum
  {
    FAULT_CLEAR,
    FAULT_OVERCURRENT,
    FAULT_ZERO_CROSS_TIMEOUT,
    FAULT_ZERO_CROSS_DELTA_OVER_LIMIT,
    FAULT_OVERVOLTAGE,
    FAULT_UNDERVOLTAGE,
    FAULT_ONBOARD_FAULT
  } systemFault_t;

  typedef enum
  {
    DIRECTION_CLOCKWISE,
    DIRECTION_ANTICLOCKWISE
  } motorDirection_t;

  uint32_t MOTOR_getRPM (void);
  systemFault_t MOTOR_getFaultCode (void);
  void MOTOR_clearFaultCode (void);
  uint8_t MOTOR_getDuty (void);
  void MOTOR_setDuty (uint8_t duty_cycle);
  void MOTOR_setDirection (motorDirection_t dir);
  motorDirection_t MOTOR_getDirection (void);

  void MOTOR_init (void);
  void MOTOR_stop (void);
  void MOTOR_start (void);

  void MOTOR_feedSupplyVoltage (uint16_t measured_voltage);
  void MOTOR_feedCurrent (int16_t measured_current);

#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_CONTROL_H */