
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


 #include "../include/tcd0.h"

/**
 * \brief Initialize TCD interface
 */
void (*TCD0_TRIG_isr_cb)(void) = NULL;
void (*TCD0_OVF_isr_cb)(void) = NULL;

void TCD0_SetTRIGIsrCallback(TCD0_cb_t cb)
{
    TCD0_TRIG_isr_cb = cb;
}

void TCD0_SetOVFIsrCallback(TCD0_cb_t cb)
{
    TCD0_OVF_isr_cb = cb;
}

int8_t TCD0_Initialize()
{
    //Clear A
    TCD0.CMPACLR = 0x10;

    //SET A
    TCD0.CMPASET = 0x07;

    //Clear B
    TCD0.CMPBCLR = 0x200;

    //SET B
    TCD0.CMPBSET = 0x00;

    //WGMODE ONERAMP; 
    TCD0.CTRLB = 0x00;

    //CMPDSEL PWMA; CMPCSEL PWMA; FIFTY disabled; AUPDATE disabled; CMPOVR disabled; 
    TCD0.CTRLC = 0x00;

    //CMPBVAL 0; CMPAVAL 0; 
    TCD0.CTRLD = 0x00;

    //DISEOC disabled; SCAPTUREB disabled; SCAPTUREA disabled; RESTART disabled; SYNC disabled; SYNCEOC disabled; 
    TCD0.CTRLE = 0x00;

    //FAULTDET disabled; DBGRUN disabled; 
    TCD0.DBGCTRL = 0x00;

    //DITHERSEL ONTIMEB; 
    TCD0.DITCTRL = 0x00;

    //DITHER 0; 
    TCD0.DITVAL = 0x00;

    //DLYPRESC DIV1; DLYTRIG CMPASET; DLYSEL OFF; 
    TCD0.DLYCTRL = 0x00;

    //DLYVAL 0; 
    TCD0.DLYVAL = 0x00;

    //CFG NEITHER; EDGE FALL_LOW; ACTION FAULT; TRIGEI disabled; 
    TCD0.EVCTRLA = 0x00;

    //CFG NEITHER; EDGE FALL_LOW; ACTION FAULT; TRIGEI disabled; 
    TCD0.EVCTRLB = 0x00;

    //CMPDEN enabled; CMPCEN enabled; CMPBEN disabled; CMPAEN disabled; CMPD enabled; CMPC enabled; CMPB disabled; CMPA disabled; 
    ccp_write_io((void*)&(TCD0.FAULTCTRL),0xCC);

    //INPUTMODE NONE; 
    TCD0.INPUTCTRLA = 0x00;

    //INPUTMODE NONE; 
    TCD0.INPUTCTRLB = 0x00;

    //TRIGB disabled; TRIGA disabled; OVF disabled; 
    TCD0.INTCTRL = 0x00;

    while ((TCD0.STATUS & TCD_ENRDY_bm) == 0); // Wait for Enable Ready to be high.

    //PWMACTB disabled; PWMACTA disabled; 
    TCD0.STATUS = 0x00;

    //CLKSEL OSCHF; CNTPRES DIV1; SYNCPRES DIV1; ENABLE disabled; 
    TCD0.CTRLA = 0x00;

    return 0;
}

ISR(TCD0_OVF_vect)
{
    /* The interrupt flag has to be cleared manually */
    TCD0.INTFLAGS = TCD_OVF_bm;
    
    /* Insert your TCD Over Flow interrupt handling code here */
    if(TCD0_OVF_isr_cb != NULL)
    {
        TCD0_OVF_isr_cb();
    }
}

ISR(TCD0_TRIG_vect)
{
    /* TRIGA interrupt flag has to be cleared manually */
    TCD0.INTFLAGS = TCD_TRIGA_bm;

    /* Insert your TCD interrupt handling code here */
    if(TCD0_TRIG_isr_cb != NULL)
    {
        TCD0_TRIG_isr_cb();
    }
}
