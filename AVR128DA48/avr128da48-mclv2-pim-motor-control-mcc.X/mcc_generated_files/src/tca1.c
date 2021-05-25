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


#include "../include/tca1.h"

void TCA1_DefaultCMP0IsrCallback(void);
void (*TCA1_CMP0_isr_cb)(void) = &TCA1_DefaultCMP0IsrCallback;
void TCA1_DefaultCMP1IsrCallback(void);
void (*TCA1_CMP1_isr_cb)(void) = &TCA1_DefaultCMP1IsrCallback;
void TCA1_DefaultCMP2IsrCallback(void);
void (*TCA1_CMP2_isr_cb)(void) = &TCA1_DefaultCMP2IsrCallback;
void TCA1_DefaultOVFIsrCallback(void);
void (*TCA1_OVF_isr_cb)(void) = &TCA1_DefaultOVFIsrCallback;

void TCA1_DefaultCMP0IsrCallback(void)
{
    //Add your ISR code here
}

void TCA1_DefaultCMP1IsrCallback(void)
{
    //Add your ISR code here
}

void TCA1_DefaultCMP2IsrCallback(void)
{
    //Add your ISR code here
}

void TCA1_DefaultOVFIsrCallback(void)
{
    //Add your ISR code here
}

void TCA1_SetOVFIsrCallback(TCA1_cb_t cb)
{
    TCA1_OVF_isr_cb = cb;
}

void TCA1_SetCMP0IsrCallback(TCA1_cb_t cb)
{
    TCA1_CMP0_isr_cb = cb;
}

void TCA1_SetCMP1IsrCallback(TCA1_cb_t cb)
{
    TCA1_CMP1_isr_cb = cb;
}

void TCA1_SetCMP2IsrCallback(TCA1_cb_t cb)
{
    TCA1_CMP2_isr_cb = cb;
}

ISR(TCA1_CMP0_vect)
{
    if (TCA1_CMP0_isr_cb != NULL)
        (*TCA1_CMP0_isr_cb)();
    
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(TCA1_CMP1_vect)
{
    if (TCA1_CMP1_isr_cb != NULL)
        (*TCA1_CMP1_isr_cb)();
    
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_CMP1_bm;
}

ISR(TCA1_CMP2_vect)
{
    if (TCA1_CMP2_isr_cb != NULL)
        (*TCA1_CMP2_isr_cb)();
    
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_CMP2_bm;
}

ISR(TCA1_OVF_vect)
{
    if (TCA1_OVF_isr_cb != NULL)
        (*TCA1_OVF_isr_cb)();
    
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}


/**
 * \brief Initialize TCA interface
 */
int8_t TCA1_Initialize()
{
    //Compare 0
    TCA1.SINGLE.CMP0 = 0x11;

    //Compare 1
    TCA1.SINGLE.CMP1 = 0x11;

    //Compare 2
    TCA1.SINGLE.CMP2 = 0x11;

    //Count
    TCA1.SINGLE.CNT = 0x00;

    //CMP2EN disabled; CMP1EN disabled; CMP0EN disabled; ALUPD disabled; WGMODE NORMAL; 
    TCA1.SINGLE.CTRLB = 0x00;

    //CMP2OV disabled; CMP1OV disabled; CMP0OV disabled; 
    TCA1.SINGLE.CTRLC = 0x00;

    //SPLITM disabled; 
    TCA1.SINGLE.CTRLD = 0x00;

    //CMD NONE; LUPD disabled; DIR disabled; 
    TCA1.SINGLE.CTRLECLR = 0x00;

    //CMD NONE; LUPD disabled; DIR UP; 
    TCA1.SINGLE.CTRLESET = 0x00;

    //CMP2BV disabled; CMP1BV disabled; CMP0BV disabled; PERBV disabled; 
    TCA1.SINGLE.CTRLFCLR = 0x00;

    //CMP2BV disabled; CMP1BV disabled; CMP0BV disabled; PERBV disabled; 
    TCA1.SINGLE.CTRLFSET = 0x00;

    //DBGRUN disabled; 
    TCA1.SINGLE.DBGCTRL = 0x00;

    //EVACTB UPDOWN; CNTBEI disabled; EVACTA CNT_POSEDGE; CNTAEI disabled; 
    TCA1.SINGLE.EVCTRL = 0x60;

    //CMP2 disabled; CMP1 disabled; CMP0 disabled; OVF disabled; 
    TCA1.SINGLE.INTCTRL = 0x00;

    //CMP2 disabled; CMP1 disabled; CMP0 disabled; OVF disabled; 
    TCA1.SINGLE.INTFLAGS = 0x00;

    //Period
    TCA1.SINGLE.PER = 0xFFFF;

    //Temporary data for 16-bit Access
    TCA1.SINGLE.TEMP = 0x00;

    //RUNSTDBY disabled; CLKSEL DIV4; ENABLE disabled; 
    TCA1.SINGLE.CTRLA = 0x04;

    return 0;
}

void TCA1_WriteTimer(uint16_t timerVal)
{
    TCA1.SINGLE.CNT=timerVal;
}

uint16_t TCA1_ReadTimer(void)
{
    uint16_t readVal;

    readVal = TCA1.SINGLE.CNT;

    return readVal;
}

void TCA1_EnableInterrupt(void)
{
        TCA1.SINGLE.INTCTRL = 1 << TCA_SINGLE_CMP0_bp /* Compare 0 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */
}
void TCA1_DisableInterrupt(void)
{
    TCA1.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp /* Compare 0 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: disabled */
}
void TCA1_ClearOverflowInterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS &= ~TCA_SINGLE_OVF_bm; /* Overflow Interrupt: disabled */
}
bool TCA1_IsOverflowInterruptEnabled(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm) > 0);
}

void TCA1_ClearCMP0InterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS &= ~TCA_SINGLE_CMP0_bm; /* Compare Channel-0 Interrupt: disabled */
}

bool TCA1_IsCMP0InterruptEnabled(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_CMP0_bm) > 0);
}

void TCA1_ClearCMP1InterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS &= ~TCA_SINGLE_CMP1_bm; /* Compare Channel-1 Interrupt: disabled */
}

bool TCA1_IsCMP1InterruptEnabled(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_CMP1_bm) > 0);
}

void TCA1_ClearCMP2InterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS &= ~TCA_SINGLE_CMP2_bm; /* Compare Channel-2 Interrupt: disabled */
}

bool TCA1_IsCMP2InterruptEnabled(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_CMP2_bm) > 0);
}
