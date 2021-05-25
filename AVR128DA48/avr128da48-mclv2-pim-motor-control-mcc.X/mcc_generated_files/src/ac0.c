/**
  @Company
    Microchip Technology Inc.

  @Description
    This Source file provides APIs.
    Generation Information :
    Driver Version    :   1.0.0
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


#include "../include/ac0.h"

void (*AC0_isr_cb)(void) = NULL;

void AC0_SetIsrCallback(AC0_cb_t cb)
{
	AC0_isr_cb = cb;
}

int8_t AC0_Initialize()
{
    
    //WINSEL UPSEL2; 
    AC0.CTRLB = 0x02;
    
    //DACREF 255; 
    AC0.DACREF = 0xFF;
    
    //INTMODE OUTSIDE; CMP disabled; 
    AC0.INTCTRL = 0x30;
    
    //INVERT disabled; INITVAL LOW; MUXPOS AINP0; MUXNEG DACREF; 
    AC0.MUXCTRL = 0x03;
    
    //RUNSTDBY disabled; OUTEN disabled; POWER PROFILE0; HYSMODE NONE; ENABLE enabled; 
    AC0.CTRLA = 0x01;
    
	return 0;
}

ISR(AC0_AC_vect)
{
	/* Insert your AC interrupt handling code here */
    if (AC0_isr_cb != NULL) {
		AC0_isr_cb();
	}
	/* The interrupt flag has to be cleared manually */
	AC0.STATUS = AC_CMPIF_bm;
}