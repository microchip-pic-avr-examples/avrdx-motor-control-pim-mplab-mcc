/**
  @Company
    Microchip Technology Inc.

  @Description
    This Source file provides APIs.
    Generation Information :
    Driver Version    :   2.0.0
*/
/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
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


#ifndef TCA1_H_INCLUDED
#define TCA1_H_INCLUDED

#include "../utils/compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize tca interface
 *
 * \return Initialization status.
 */
    
typedef void (*TCA1_cb_t)(void);    

int8_t TCA1_Initialize();
void TCA1_SetOVFIsrCallback(TCA1_cb_t cb);
void TCA1_SetCMP0IsrCallback(TCA1_cb_t cb);
void TCA1_SetCMP1IsrCallback(TCA1_cb_t cb);
void TCA1_SetCMP2IsrCallback(TCA1_cb_t cb);
void TCA1_EnableInterrupt(void);
void TCA1_DisableInterrupt(void);
uint16_t TCA1_ReadTimer(void);
void TCA1_WriteTimer(uint16_t timerVal);
void TCA1_ClearOverflowInterruptFlag(void);
bool TCA1_IsOverflowInterruptEnabled(void);
void TCA1_ClearCMP0InterruptFlag(void);
bool TCA1_IsCMP0InterruptEnabled(void);
void TCA1_ClearCMP1InterruptFlag(void);
bool TCA1_IsCMP1InterruptEnabled(void);
void TCA1_ClearCMP2InterruptFlag(void);
bool TCA1_IsCMP2InterruptEnabled(void);
#ifdef __cplusplus
}
#endif

#endif /* TCA1_H_INCLUDED */