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

#ifndef BOARD_PARAMS_H
#define	BOARD_PARAMS_H

#ifdef	__cplusplus
extern "C"
{
#endif

  /*Over-current hardware parameters*/
  /* Offset for the 0A reading in mV */
#define MOTOR_CURRENT_SHUNT_VOLTAGE_OFFSET  1650
  /* Current shunt sensitivity (mv/A) */
#define MOTOR_CURRENT_SHUNT_SENS            375    
#define MOTOR_CURRENT_MULTIPLIER_MA         1000

#define SUPPLY_VOLTAGE_DIVIDER_RATIO 16 

#define TCD_DEADTIME 18 //get it from the excel sheet 

#define VDD_VREF 3300 //vdd supply in mV  

#ifdef	__cplusplus
}
#endif

#endif	/* BOARD_PARAMS_H */

