/*
 * control.c
 *
 *  Created on: Jan 1, 2016
 *      Author: billwang
 *
 *      This file should contain tools for controlling the quadcopter
 *      In other words, This file reads from and writes to two important sets of data
 *
 *      		1. READ: g_pfData[16], which are 16 floats that contains all position/acceleration data from MPU9150 sensor,
 *      		More details could be found in CompDCM() function, g_pfData will be updated when CompDCM(&g_pfData) is
 *      		called
 *
 *      		2. WRITE: g_ESC?_Speed, which are global 8 bit int [0,255] to set ESC speeds
 *
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
#include "pid.h"
#include "control.h"

//*****************************************************************************
//
// Param
//
//*****************************************************************************
#define WakeInterval 100		// in milliseconds

//*****************************************************************************
// global array used to store compdcm data,
// g_pfData[9]	-roll
// g_pfData[10]	-pitch
// g_pfData[11]	-raw
//*****************************************************************************
float g_pfData[16];

//*****************************************************************************
//
// ESC Speed Values
//
//*****************************************************************************
uint8_t g_ESC0_Speed;
uint8_t g_ESC1_Speed;
uint8_t g_ESC2_Speed;
uint8_t g_ESC3_Speed;

//*****************************************************************************
//
// The function for starting the "system clock" - WTimer0.
//
// This timer keeps track of the time the entire system has started.
//
//*****************************************************************************
void
SysTimerConfig(){

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	ROM_TimerConfigure(WTIMER0_BASE, TIMER_CFG_PERIODIC); // for clearing the timer
	ROM_TimerConfigure(WTIMER0_BASE, TIMER_CFG_PERIODIC_UP); // reset to count up, and timer is reset to 0
	ROM_TimerEnable( WTIMER0_BASE, TIMER_A);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_WTIMER0);
}

//*****************************************************************************
//
// The function returns the current time in ms
//
//*****************************************************************************
unsigned long
MilliSecondsTime()
{
	return ROM_TimerValueGet(WTIMER0_BASE, TIMER_A)/40000;
}

//*****************************************************************************
//
// The function for starting the interrupt - Timer0A.
//
// This timer wakes the cpu up for reading dcm value and control algorithm
//
//*****************************************************************************
void
WakeAlarmConfig(){

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, 400000000/WakeInterval);
	// timer counts to 400000000/100 and trigger interrrupt
	// with 40M clock, it takes 100 millisecond to count up and reset
	// so the calcs will be done every 100 millisecond
	TimerIntRegister(TIMER0_BASE, TIMER_A, IntWakeUp);
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	ROM_IntEnable(INT_TIMER0A);
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER0);
}

//*****************************************************************************
//
// The interrupt handler for the control timer.
//
//*****************************************************************************
void
IntWakeUp(void)
{
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);            // clear the timer interrupt
	/*
	 * Code can be written in this interrupt directly
	 *
	 * OR
	 *
	 * System will stay awake after this interrupt and before put to sleep again
	 *
	 */
}


