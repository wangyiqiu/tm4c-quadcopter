/*
 * main.c
 *
 * This is the main program file for a quadcopter
 *
 * Created on: 2015/7/26
 * Author: Bill Yiqiu Wang
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "ble_mini.h"
#include "compdcm_9150.h"
#include "driverlib/timer.h"
#include "esc.h"
#include "control.h"

//*****************************************************************************
// char buffer
//*****************************************************************************
uint32_t receiveChar;
int32_t receiveChar2;
int32_t receiveChar3;
int32_t receiveChar4;
char sendChar;

int
main(void)
{

    //*****************************************************************************
    //
    // SETUP
    //
    //*****************************************************************************
    // Setup the system clock to run at 40 Mhz from PLL with crystal reference
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    BLEMiniConfig();
    //ConfigureConsoleUART();
    ControlSetup();
    ESCConfig(); // Config four esc outputs

    //*****************************************************************************
    //
    // MAIN LOOP
    //
    //*****************************************************************************
    while(1)
    {
//    	if (UARTCharsAvail(UART1_BASE)) {
//    		curChar=UARTCharGet(UART1_BASE);
//    		UARTCharPut(UART0_BASE,curChar);
//    		UARTCharPut(UART0_BASE,curChar >> 8);
//    		UARTCharPut(UART0_BASE,curChar >> 16);
//    		UARTCharPut(UART0_BASE,curChar >> 24);
//    	}
//    	while(UARTCharsAvail(UART1_BASE)){
//    		receiveChar=UARTCharGetNonBlocking(UART1_BASE);
//    		UARTCharPutNonBlocking(UART0_BASE, receiveChar);
//    	}
//    	while(UARTCharsAvail(UART0_BASE)){
//    		sendChar=UARTCharGetNonBlocking(UART0_BASE);
//			UARTCharPutNonBlocking(UART1_BASE, sendChar);
//    	}
    	COMPDCM(g_pfData);
    	ROM_SysCtlSleep(); // put system to sleep and wait for balancing timer int to wake up
    }
}
