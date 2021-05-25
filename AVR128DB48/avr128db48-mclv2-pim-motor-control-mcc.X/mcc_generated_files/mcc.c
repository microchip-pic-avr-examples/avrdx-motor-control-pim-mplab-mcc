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


#include "mcc.h"

/**
 * Initializes MCU, drivers and middleware in the project
**/
void SYSTEM_Initialize(void)
{
    PIN_MANAGER_Initialize();
    //CPU_Initialize();
    BOD_Initialize();
    WDT_Initialize();
    CLKCTRL_Initialize();
    SLPCTRL_Initialize();
    OPAMP_Initialize();
    AC0_Initialize();
    ADC0_Initialize();
    TCA0_Initialize();
    CCL_Initialize();
    TCB0_Initialize();
    AC2_Initialize();
    AC1_Initialize();
    TCD0_Initialize();
    TCA1_Initialize();
    VREF_0_Initialize();
    CPUINT_Initialize();
    USART0_Initialize();
}

/**
 * \brief Initialize bod interface
 */
int8_t BOD_Initialize()
{
    //SLEEP DIS; 
    ccp_write_io((void*)&(BOD.CTRLA),0x00);

    //VLMCFG FALLING; VLMIE disabled; 
	BOD.INTCTRL = 0x00;

    //VLMLVL OFF; 
	BOD.VLMCTRLA = 0x00;

	return 0;
}

ISR(BOD_VLM_vect)
{
	/* Insert your AC interrupt handling code here */

	/* The interrupt flag has to be cleared manually */
	BOD.INTFLAGS = BOD_VLMIE_bm;
}

/**
 * \brief Initialize clkctrl interface
 */
int8_t CLKCTRL_Initialize()
{
    //RUNSTDBY disabled; 
    ccp_write_io((void*)&(CLKCTRL.OSC32KCTRLA),0x00);

    //RUNSTDBY disabled; CSUT 1K; SEL disabled; LPMODE disabled; ENABLE disabled; 
    ccp_write_io((void*)&(CLKCTRL.XOSC32KCTRLA),0x00);

    //PDIV 6X; PEN disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKCTRLB),0x10);

    //CFDSRC CLKMAIN; CFDTST disabled; CFDEN disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKCTRLC),0x00);

    //INTTYPE INT; CFD disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKINTCTRL),0x00);

    //CLKOUT disabled; CLKSEL OSCHF; 
    ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),0x00);

    //PLLS disabled; EXTS disabled; XOSC32KS disabled; OSC32KS disabled; OSCHFS disabled; SOSC disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKSTATUS),0x00);

    //RUNSTDBY disabled; FREQSEL 24M; AUTOTUNE disabled; 
    ccp_write_io((void*)&(CLKCTRL.OSCHFCTRLA),0x24); 

	return 0;
}

/**
 * \brief Initialize slpctrl interface
 */
int8_t SLPCTRL_Initialize()
{
    //SMODE IDLE; SEN disabled; 
    ccp_write_io((void*)&(SLPCTRL.CTRLA),0x00);

    return 0;
}

/**
 * \brief Initialize wdt interface
 */
 
int8_t WDT_Initialize()
{
    //WINDOW OFF; PERIOD OFF; 
    ccp_write_io((void*)&(WDT.CTRLA),0x00);

	return 0;
}
/*TODO comment will remove once complier support will be available. 
int8_t CPU_Initialize()
{
    return 0;

}
*/