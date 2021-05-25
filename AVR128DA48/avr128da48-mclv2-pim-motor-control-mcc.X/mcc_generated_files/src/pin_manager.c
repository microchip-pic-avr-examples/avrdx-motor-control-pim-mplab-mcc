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


#include "../include/pin_manager.h"
static void (*PORTB_PB3_InterruptHandler)(void);
static void (*PORTD_PD5_InterruptHandler)(void);
static void (*PORTD_PH_C_V_InterruptHandler)(void);
static void (*PORTB_HALL_A_InterruptHandler)(void);
static void (*PORTD_VREF_InterruptHandler)(void);
static void (*PORTB_HALL_B_InterruptHandler)(void);
static void (*PORTD_PH_B_V_InterruptHandler)(void);
static void (*PORTB_HALL_C_InterruptHandler)(void);
static void (*PORTE_PE0_InterruptHandler)(void);
static void (*PORTC_DBG_LED1_InterruptHandler)(void);
static void (*PORTC_BUS_CURRENT_FLT_InterruptHandler)(void);
static void (*PORTA_PA0_InterruptHandler)(void);
static void (*PORTC_LIN_FAULT_InterruptHandler)(void);
static void (*PORTA_LIN_CS_InterruptHandler)(void);
static void (*PORTC_PC3_InterruptHandler)(void);
static void (*PORTA_PA1_InterruptHandler)(void);
static void (*PORTC_IO_PC6_InterruptHandler)(void);
static void (*PORTA_IO_PA4_InterruptHandler)(void);
static void (*PORTC_IO_PC5_InterruptHandler)(void);
static void (*PORTA_PA3_InterruptHandler)(void);
static void (*PORTA_IO_PA6_InterruptHandler)(void);
static void (*PORTC_HOME_InterruptHandler)(void);
static void (*PORTA_DBG_LED2_InterruptHandler)(void);
static void (*PORTA_IO_PA7_InterruptHandler)(void);
static void (*PORTF_IO_PF1_InterruptHandler)(void);
static void (*PORTF_IO_PF0_InterruptHandler)(void);
static void (*PORTF_PF3_InterruptHandler)(void);
static void (*PORTD_PD1_InterruptHandler)(void);
static void (*PORTF_IO_PF2_InterruptHandler)(void);
static void (*PORTD_PD0_InterruptHandler)(void);
static void (*PORTF_BTN_1_InterruptHandler)(void);
static void (*PORTD_PH_A_V_InterruptHandler)(void);
static void (*PORTF_BTN_2_InterruptHandler)(void);
static void (*PORTD_PD2_InterruptHandler)(void);

void PORT_Initialize(void);

void PIN_MANAGER_Initialize()
{
    PORT_Initialize();

    /* DIR Registers Initialization */
    PORTA.DIR = 0xF9;
    PORTB.DIR = 0x08;
    PORTC.DIR = 0x69;
    PORTD.DIR = 0x00;
    PORTE.DIR = 0x00;
    PORTF.DIR = 0x08;

    /* OUT Registers Initialization */
    PORTA.OUT = 0x00;
    PORTB.OUT = 0x00;
    PORTC.OUT = 0x00;
    PORTD.OUT = 0x00;
    PORTE.OUT = 0x00;
    PORTF.OUT = 0x00;

    /* PINxCTRL registers Initialization */
    PORTA.PIN0CTRL = 0x00;
    PORTA.PIN1CTRL = 0x00;
    PORTA.PIN2CTRL = 0x00;
    PORTA.PIN3CTRL = 0x00;
    PORTA.PIN4CTRL = 0x00;
    PORTA.PIN5CTRL = 0x04;
    PORTA.PIN6CTRL = 0x00;
    PORTA.PIN7CTRL = 0x00;
    PORTB.PIN0CTRL = 0x00;
    PORTB.PIN1CTRL = 0x00;
    PORTB.PIN2CTRL = 0x00;
    PORTB.PIN3CTRL = 0x00;
    PORTB.PIN4CTRL = 0x00;
    PORTB.PIN5CTRL = 0x00;
    PORTB.PIN6CTRL = 0x00;
    PORTB.PIN7CTRL = 0x00;
    PORTC.PIN0CTRL = 0x00;
    PORTC.PIN1CTRL = 0x00;
    PORTC.PIN2CTRL = 0x03;
    PORTC.PIN3CTRL = 0x00;
    PORTC.PIN4CTRL = 0x00;
    PORTC.PIN5CTRL = 0x00;
    PORTC.PIN6CTRL = 0x00;
    PORTC.PIN7CTRL = 0x00;
    PORTD.PIN0CTRL = 0x04;
    PORTD.PIN1CTRL = 0x04;
    PORTD.PIN2CTRL = 0x04;
    PORTD.PIN3CTRL = 0x04;
    PORTD.PIN4CTRL = 0x04;
    PORTD.PIN5CTRL = 0x04;
    PORTD.PIN6CTRL = 0x04;
    PORTD.PIN7CTRL = 0x04;
    PORTE.PIN0CTRL = 0x04;
    PORTE.PIN1CTRL = 0x00;
    PORTE.PIN2CTRL = 0x00;
    PORTE.PIN3CTRL = 0x00;
    PORTE.PIN4CTRL = 0x00;
    PORTE.PIN5CTRL = 0x00;
    PORTE.PIN6CTRL = 0x00;
    PORTE.PIN7CTRL = 0x00;
    PORTF.PIN0CTRL = 0x00;
    PORTF.PIN1CTRL = 0x00;
    PORTF.PIN2CTRL = 0x04;
    PORTF.PIN3CTRL = 0x00;
    PORTF.PIN4CTRL = 0x03;
    PORTF.PIN5CTRL = 0x03;
    PORTF.PIN6CTRL = 0x00;
    PORTF.PIN7CTRL = 0x00;

    /* Multi-pin Config registers Initialization */
    PORTA.PINCONFIG = 0x00;
    PORTA.PINCTRLCLR = 0x00;
    PORTA.PINCTRLSET = 0x00;
    PORTA.PINCTRLUPD = 0x00;
    PORTB.PINCONFIG = 0x00;
    PORTB.PINCTRLCLR = 0x00;
    PORTB.PINCTRLSET = 0x00;
    PORTB.PINCTRLUPD = 0x00;
    PORTC.PINCONFIG = 0x00;
    PORTC.PINCTRLCLR = 0x00;
    PORTC.PINCTRLSET = 0x00;
    PORTC.PINCTRLUPD = 0x00;
    PORTD.PINCONFIG = 0x00;
    PORTD.PINCTRLCLR = 0x00;
    PORTD.PINCTRLSET = 0x00;
    PORTD.PINCTRLUPD = 0x00;
    PORTE.PINCONFIG = 0x00;
    PORTE.PINCTRLCLR = 0x00;
    PORTE.PINCTRLSET = 0x00;
    PORTE.PINCTRLUPD = 0x00;
    PORTF.PINCONFIG = 0x00;
    PORTF.PINCTRLCLR = 0x00;
    PORTF.PINCTRLSET = 0x00;
    PORTF.PINCTRLUPD = 0x00;

    /* PORTMUX Initialization */
    PORTMUX.ACROUTEA = 0x00;
    PORTMUX.CCLROUTEA = 0x00;
    PORTMUX.EVSYSROUTEA = 0x00;
    PORTMUX.SPIROUTEA = 0x00;
    PORTMUX.TCAROUTEA = 0x00;
    PORTMUX.TCBROUTEA = 0x00;
    PORTMUX.TCDROUTEA = 0x00;
    PORTMUX.TWIROUTEA = 0x00;
    PORTMUX.USARTROUTEA = 0x00;
    PORTMUX.USARTROUTEB = 0x00;
    PORTMUX.ZCDROUTEA = 0x00;

    // register default ISC callback functions at runtime; use these methods to register a custom function
    PORTB_PB3_SetInterruptHandler(PORTB_PB3_DefaultInterruptHandler);
    PORTD_PD5_SetInterruptHandler(PORTD_PD5_DefaultInterruptHandler);
    PORTD_PH_C_V_SetInterruptHandler(PORTD_PH_C_V_DefaultInterruptHandler);
    PORTB_HALL_A_SetInterruptHandler(PORTB_HALL_A_DefaultInterruptHandler);
    PORTD_VREF_SetInterruptHandler(PORTD_VREF_DefaultInterruptHandler);
    PORTB_HALL_B_SetInterruptHandler(PORTB_HALL_B_DefaultInterruptHandler);
    PORTD_PH_B_V_SetInterruptHandler(PORTD_PH_B_V_DefaultInterruptHandler);
    PORTB_HALL_C_SetInterruptHandler(PORTB_HALL_C_DefaultInterruptHandler);
    PORTE_PE0_SetInterruptHandler(PORTE_PE0_DefaultInterruptHandler);
    PORTC_DBG_LED1_SetInterruptHandler(PORTC_DBG_LED1_DefaultInterruptHandler);
    PORTC_BUS_CURRENT_FLT_SetInterruptHandler(PORTC_BUS_CURRENT_FLT_DefaultInterruptHandler);
    PORTA_PA0_SetInterruptHandler(PORTA_PA0_DefaultInterruptHandler);
    PORTC_LIN_FAULT_SetInterruptHandler(PORTC_LIN_FAULT_DefaultInterruptHandler);
    PORTA_LIN_CS_SetInterruptHandler(PORTA_LIN_CS_DefaultInterruptHandler);
    PORTC_PC3_SetInterruptHandler(PORTC_PC3_DefaultInterruptHandler);
    PORTA_PA1_SetInterruptHandler(PORTA_PA1_DefaultInterruptHandler);
    PORTC_IO_PC6_SetInterruptHandler(PORTC_IO_PC6_DefaultInterruptHandler);
    PORTA_IO_PA4_SetInterruptHandler(PORTA_IO_PA4_DefaultInterruptHandler);
    PORTC_IO_PC5_SetInterruptHandler(PORTC_IO_PC5_DefaultInterruptHandler);
    PORTA_PA3_SetInterruptHandler(PORTA_PA3_DefaultInterruptHandler);
    PORTA_IO_PA6_SetInterruptHandler(PORTA_IO_PA6_DefaultInterruptHandler);
    PORTC_HOME_SetInterruptHandler(PORTC_HOME_DefaultInterruptHandler);
    PORTA_DBG_LED2_SetInterruptHandler(PORTA_DBG_LED2_DefaultInterruptHandler);
    PORTA_IO_PA7_SetInterruptHandler(PORTA_IO_PA7_DefaultInterruptHandler);
    PORTF_IO_PF1_SetInterruptHandler(PORTF_IO_PF1_DefaultInterruptHandler);
    PORTF_IO_PF0_SetInterruptHandler(PORTF_IO_PF0_DefaultInterruptHandler);
    PORTF_PF3_SetInterruptHandler(PORTF_PF3_DefaultInterruptHandler);
    PORTD_PD1_SetInterruptHandler(PORTD_PD1_DefaultInterruptHandler);
    PORTF_IO_PF2_SetInterruptHandler(PORTF_IO_PF2_DefaultInterruptHandler);
    PORTD_PD0_SetInterruptHandler(PORTD_PD0_DefaultInterruptHandler);
    PORTF_BTN_1_SetInterruptHandler(PORTF_BTN_1_DefaultInterruptHandler);
    PORTD_PH_A_V_SetInterruptHandler(PORTD_PH_A_V_DefaultInterruptHandler);
    PORTF_BTN_2_SetInterruptHandler(PORTF_BTN_2_DefaultInterruptHandler);
    PORTD_PD2_SetInterruptHandler(PORTD_PD2_DefaultInterruptHandler);
}

void PORT_Initialize(void)
{
    /* On AVR devices all peripherals are enable from power on reset, this
     * disables all peripherals to save power. Driver shall enable
     * peripheral if used */

    /* Set all pins to low power mode */
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTE + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTF + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTA + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTB + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTC + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTD + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
}

/**
  Allows selecting an interrupt handler for PORTB_PB3 at application runtime
*/
void PORTB_PB3_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTB_PB3_InterruptHandler = interruptHandler;
}

void PORTB_PB3_DefaultInterruptHandler(void)
{
    // add your PORTB_PB3 interrupt custom code
    // or set custom function using PORTB_PB3_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_PD5 at application runtime
*/
void PORTD_PD5_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_PD5_InterruptHandler = interruptHandler;
}

void PORTD_PD5_DefaultInterruptHandler(void)
{
    // add your PORTD_PD5 interrupt custom code
    // or set custom function using PORTD_PD5_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_PH_C_V at application runtime
*/
void PORTD_PH_C_V_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_PH_C_V_InterruptHandler = interruptHandler;
}

void PORTD_PH_C_V_DefaultInterruptHandler(void)
{
    // add your PORTD_PH_C_V interrupt custom code
    // or set custom function using PORTD_PH_C_V_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTB_HALL_A at application runtime
*/
void PORTB_HALL_A_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTB_HALL_A_InterruptHandler = interruptHandler;
}

void PORTB_HALL_A_DefaultInterruptHandler(void)
{
    // add your PORTB_HALL_A interrupt custom code
    // or set custom function using PORTB_HALL_A_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_VREF at application runtime
*/
void PORTD_VREF_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_VREF_InterruptHandler = interruptHandler;
}

void PORTD_VREF_DefaultInterruptHandler(void)
{
    // add your PORTD_VREF interrupt custom code
    // or set custom function using PORTD_VREF_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTB_HALL_B at application runtime
*/
void PORTB_HALL_B_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTB_HALL_B_InterruptHandler = interruptHandler;
}

void PORTB_HALL_B_DefaultInterruptHandler(void)
{
    // add your PORTB_HALL_B interrupt custom code
    // or set custom function using PORTB_HALL_B_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_PH_B_V at application runtime
*/
void PORTD_PH_B_V_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_PH_B_V_InterruptHandler = interruptHandler;
}

void PORTD_PH_B_V_DefaultInterruptHandler(void)
{
    // add your PORTD_PH_B_V interrupt custom code
    // or set custom function using PORTD_PH_B_V_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTB_HALL_C at application runtime
*/
void PORTB_HALL_C_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTB_HALL_C_InterruptHandler = interruptHandler;
}

void PORTB_HALL_C_DefaultInterruptHandler(void)
{
    // add your PORTB_HALL_C interrupt custom code
    // or set custom function using PORTB_HALL_C_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTE_PE0 at application runtime
*/
void PORTE_PE0_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTE_PE0_InterruptHandler = interruptHandler;
}

void PORTE_PE0_DefaultInterruptHandler(void)
{
    // add your PORTE_PE0 interrupt custom code
    // or set custom function using PORTE_PE0_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_DBG_LED1 at application runtime
*/
void PORTC_DBG_LED1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_DBG_LED1_InterruptHandler = interruptHandler;
}

void PORTC_DBG_LED1_DefaultInterruptHandler(void)
{
    // add your PORTC_DBG_LED1 interrupt custom code
    // or set custom function using PORTC_DBG_LED1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_BUS_CURRENT_FLT at application runtime
*/
void PORTC_BUS_CURRENT_FLT_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_BUS_CURRENT_FLT_InterruptHandler = interruptHandler;
}

void PORTC_BUS_CURRENT_FLT_DefaultInterruptHandler(void)
{
    // add your PORTC_BUS_CURRENT_FLT interrupt custom code
    // or set custom function using PORTC_BUS_CURRENT_FLT_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA0 at application runtime
*/
void PORTA_PA0_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA0_InterruptHandler = interruptHandler;
}

void PORTA_PA0_DefaultInterruptHandler(void)
{
    // add your PORTA_PA0 interrupt custom code
    // or set custom function using PORTA_PA0_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_LIN_FAULT at application runtime
*/
void PORTC_LIN_FAULT_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_LIN_FAULT_InterruptHandler = interruptHandler;
}

void PORTC_LIN_FAULT_DefaultInterruptHandler(void)
{
    // add your PORTC_LIN_FAULT interrupt custom code
    // or set custom function using PORTC_LIN_FAULT_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_LIN_CS at application runtime
*/
void PORTA_LIN_CS_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_LIN_CS_InterruptHandler = interruptHandler;
}

void PORTA_LIN_CS_DefaultInterruptHandler(void)
{
    // add your PORTA_LIN_CS interrupt custom code
    // or set custom function using PORTA_LIN_CS_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_PC3 at application runtime
*/
void PORTC_PC3_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_PC3_InterruptHandler = interruptHandler;
}

void PORTC_PC3_DefaultInterruptHandler(void)
{
    // add your PORTC_PC3 interrupt custom code
    // or set custom function using PORTC_PC3_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA1 at application runtime
*/
void PORTA_PA1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA1_InterruptHandler = interruptHandler;
}

void PORTA_PA1_DefaultInterruptHandler(void)
{
    // add your PORTA_PA1 interrupt custom code
    // or set custom function using PORTA_PA1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_IO_PC6 at application runtime
*/
void PORTC_IO_PC6_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_IO_PC6_InterruptHandler = interruptHandler;
}

void PORTC_IO_PC6_DefaultInterruptHandler(void)
{
    // add your PORTC_IO_PC6 interrupt custom code
    // or set custom function using PORTC_IO_PC6_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_IO_PA4 at application runtime
*/
void PORTA_IO_PA4_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_IO_PA4_InterruptHandler = interruptHandler;
}

void PORTA_IO_PA4_DefaultInterruptHandler(void)
{
    // add your PORTA_IO_PA4 interrupt custom code
    // or set custom function using PORTA_IO_PA4_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_IO_PC5 at application runtime
*/
void PORTC_IO_PC5_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_IO_PC5_InterruptHandler = interruptHandler;
}

void PORTC_IO_PC5_DefaultInterruptHandler(void)
{
    // add your PORTC_IO_PC5 interrupt custom code
    // or set custom function using PORTC_IO_PC5_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA3 at application runtime
*/
void PORTA_PA3_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA3_InterruptHandler = interruptHandler;
}

void PORTA_PA3_DefaultInterruptHandler(void)
{
    // add your PORTA_PA3 interrupt custom code
    // or set custom function using PORTA_PA3_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_IO_PA6 at application runtime
*/
void PORTA_IO_PA6_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_IO_PA6_InterruptHandler = interruptHandler;
}

void PORTA_IO_PA6_DefaultInterruptHandler(void)
{
    // add your PORTA_IO_PA6 interrupt custom code
    // or set custom function using PORTA_IO_PA6_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_HOME at application runtime
*/
void PORTC_HOME_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_HOME_InterruptHandler = interruptHandler;
}

void PORTC_HOME_DefaultInterruptHandler(void)
{
    // add your PORTC_HOME interrupt custom code
    // or set custom function using PORTC_HOME_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_DBG_LED2 at application runtime
*/
void PORTA_DBG_LED2_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_DBG_LED2_InterruptHandler = interruptHandler;
}

void PORTA_DBG_LED2_DefaultInterruptHandler(void)
{
    // add your PORTA_DBG_LED2 interrupt custom code
    // or set custom function using PORTA_DBG_LED2_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_IO_PA7 at application runtime
*/
void PORTA_IO_PA7_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_IO_PA7_InterruptHandler = interruptHandler;
}

void PORTA_IO_PA7_DefaultInterruptHandler(void)
{
    // add your PORTA_IO_PA7 interrupt custom code
    // or set custom function using PORTA_IO_PA7_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_IO_PF1 at application runtime
*/
void PORTF_IO_PF1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_IO_PF1_InterruptHandler = interruptHandler;
}

void PORTF_IO_PF1_DefaultInterruptHandler(void)
{
    // add your PORTF_IO_PF1 interrupt custom code
    // or set custom function using PORTF_IO_PF1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_IO_PF0 at application runtime
*/
void PORTF_IO_PF0_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_IO_PF0_InterruptHandler = interruptHandler;
}

void PORTF_IO_PF0_DefaultInterruptHandler(void)
{
    // add your PORTF_IO_PF0 interrupt custom code
    // or set custom function using PORTF_IO_PF0_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_PF3 at application runtime
*/
void PORTF_PF3_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_PF3_InterruptHandler = interruptHandler;
}

void PORTF_PF3_DefaultInterruptHandler(void)
{
    // add your PORTF_PF3 interrupt custom code
    // or set custom function using PORTF_PF3_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_PD1 at application runtime
*/
void PORTD_PD1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_PD1_InterruptHandler = interruptHandler;
}

void PORTD_PD1_DefaultInterruptHandler(void)
{
    // add your PORTD_PD1 interrupt custom code
    // or set custom function using PORTD_PD1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_IO_PF2 at application runtime
*/
void PORTF_IO_PF2_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_IO_PF2_InterruptHandler = interruptHandler;
}

void PORTF_IO_PF2_DefaultInterruptHandler(void)
{
    // add your PORTF_IO_PF2 interrupt custom code
    // or set custom function using PORTF_IO_PF2_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_PD0 at application runtime
*/
void PORTD_PD0_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_PD0_InterruptHandler = interruptHandler;
}

void PORTD_PD0_DefaultInterruptHandler(void)
{
    // add your PORTD_PD0 interrupt custom code
    // or set custom function using PORTD_PD0_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_BTN_1 at application runtime
*/
void PORTF_BTN_1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_BTN_1_InterruptHandler = interruptHandler;
}

void PORTF_BTN_1_DefaultInterruptHandler(void)
{
    // add your PORTF_BTN_1 interrupt custom code
    // or set custom function using PORTF_BTN_1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_PH_A_V at application runtime
*/
void PORTD_PH_A_V_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_PH_A_V_InterruptHandler = interruptHandler;
}

void PORTD_PH_A_V_DefaultInterruptHandler(void)
{
    // add your PORTD_PH_A_V interrupt custom code
    // or set custom function using PORTD_PH_A_V_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_BTN_2 at application runtime
*/
void PORTF_BTN_2_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_BTN_2_InterruptHandler = interruptHandler;
}

void PORTF_BTN_2_DefaultInterruptHandler(void)
{
    // add your PORTF_BTN_2 interrupt custom code
    // or set custom function using PORTF_BTN_2_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_PD2 at application runtime
*/
void PORTD_PD2_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_PD2_InterruptHandler = interruptHandler;
}

void PORTD_PD2_DefaultInterruptHandler(void)
{
    // add your PORTD_PD2 interrupt custom code
    // or set custom function using PORTD_PD2_SetInterruptHandler()
}
ISR(PORTF_PORT_vect)
{  
    // Call the interrupt handler for the callback registered at runtime
    if(VPORTF.INTFLAGS & PORT_INT1_bm)
    {
       PORTF_IO_PF1_InterruptHandler();
    }
    if(VPORTF.INTFLAGS & PORT_INT0_bm)
    {
       PORTF_IO_PF0_InterruptHandler();
    }
    if(VPORTF.INTFLAGS & PORT_INT3_bm)
    {
       PORTF_PF3_InterruptHandler();
    }
    if(VPORTF.INTFLAGS & PORT_INT2_bm)
    {
       PORTF_IO_PF2_InterruptHandler();
    }
    if(VPORTF.INTFLAGS & PORT_INT5_bm)
    {
       PORTF_BTN_1_InterruptHandler();
    }
    if(VPORTF.INTFLAGS & PORT_INT4_bm)
    {
       PORTF_BTN_2_InterruptHandler();
    }

    /* Clear interrupt flags */
    VPORTF.INTFLAGS = 0xff;
}

ISR(PORTC_PORT_vect)
{  
    // Call the interrupt handler for the callback registered at runtime
    if(VPORTC.INTFLAGS & PORT_INT0_bm)
    {
       PORTC_DBG_LED1_InterruptHandler();
    }
    if(VPORTC.INTFLAGS & PORT_INT2_bm)
    {
       PORTC_BUS_CURRENT_FLT_InterruptHandler();
    }
    if(VPORTC.INTFLAGS & PORT_INT1_bm)
    {
       PORTC_LIN_FAULT_InterruptHandler();
    }
    if(VPORTC.INTFLAGS & PORT_INT3_bm)
    {
       PORTC_PC3_InterruptHandler();
    }
    if(VPORTC.INTFLAGS & PORT_INT6_bm)
    {
       PORTC_IO_PC6_InterruptHandler();
    }
    if(VPORTC.INTFLAGS & PORT_INT5_bm)
    {
       PORTC_IO_PC5_InterruptHandler();
    }
    if(VPORTC.INTFLAGS & PORT_INT7_bm)
    {
       PORTC_HOME_InterruptHandler();
    }

    /* Clear interrupt flags */
    VPORTC.INTFLAGS = 0xff;
}

