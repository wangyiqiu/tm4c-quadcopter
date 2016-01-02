/*
 * control.c
 *
 *  Created on: Jan 1, 2016
 *      Author: billwang
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

//*****************************************************************************
// global array used to store compdcm data,
// g_pfData[9]	-roll
// g_pfData[10]	-pitch
// g_pfData[11]	-raw
//*****************************************************************************
float g_pfData[16];

//*****************************************************************************
//
// PWM Pulse Widths Params
//
//*****************************************************************************
uint32_t PWMPulseWidth_0;
uint32_t PWMPulseWidth_1;
uint32_t PWMPulseWidth_2;
uint32_t PWMPulseWidth_3;

//*****************************************************************************
//
// Declaration The structure that contains the state of a roll angle PID instance.
//
//*****************************************************************************
pidInstance g_roll_pid;


//*****************************************************************************
//
// Definition The structure that contains the state of a pitch angle PID instance.
//
//*****************************************************************************
pidInstance g_pitch_pid;

//*****************************************************************************
//
// The function for starting the "system clock" - WTimer0.
//
// This timer keeps track of the time the entire system has started.
//
//*****************************************************************************
void
SysTimerStart(){

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
millis()
{
	return ROM_TimerValueGet(WTIMER0_BASE, TIMER_A)/40000;
}


//*****************************************************************************
//
// The interrupt handler for the control timer.
//
//*****************************************************************************
void
IntBalancing(void)
{
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);            // clear the timer interrupt
	// the interrupt also put system out of sleep mode
}

//*****************************************************************************
//
// The function for starting the flight balance clock - Timer0A.
//
// In this timer interrupt, control calculation will be done
//
//*****************************************************************************
void
BalancingStart(){
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, 4000000);
	// timer counts to 4000000 and trigger interrrupt
	// with 40M clock, it takes 100 millisecond to count up and reset
	// so the calcs will be done every 100 millisecond
	TimerIntRegister(TIMER0_BASE, TIMER_A, IntBalancing);
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	ROM_IntEnable(INT_TIMER0A);
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER0);
}

//*****************************************************************************
//
// The function to set up the control system
//
//*****************************************************************************
void
ControlSetup(){
    Compdcm9150Config();
    SysTimerStart();
    PIDConfig(0,0,0,1,0.1,0.1,DIRECT, &g_roll_pid); // roll PID config
    PIDConfig(0,0,0,1,0.1,0.1,DIRECT, &g_pitch_pid); // pitch PID config
    BalancingStart();
}

//*****************************************************************************
//
// The function to calculate control result
//
//*****************************************************************************
void
ControlCompute(){

	COMPDCM(g_pfData); // compute YPR

	g_roll_pid.myInput = g_pfData[9]; // update current roll val

	g_pitch_pid.myInput = g_pfData[10]; // update current pitch val

	// set controller direction
	if (g_pfData[9] < 0) {
		SetControllerDirection(DIRECT, &g_roll_pid);
	} else {
		SetControllerDirection(REVERSE, &g_roll_pid);
	}

	if (g_pfData[10] < 0) {
		SetControllerDirection(DIRECT, &g_pitch_pid);
	} else {
		SetControllerDirection(REVERSE, &g_pitch_pid);
	}

	g_roll_pid.mySetpoint = 0; // to balance the copter , setpoint is roll = 0
	g_pitch_pid.mySetpoint = 0; // to balance the copter , setpoint is pitch = 0

	ComputePID(&g_roll_pid);
	ComputePID(&g_pitch_pid);

	if (g_pfData[9] < 0 && g_pfData[10] < 0) { // both direct
		PWMPulseWidth_0 = 50 + g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
		PWMPulseWidth_1 = 50 + g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
		PWMPulseWidth_2 = 50 - g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
		PWMPulseWidth_3 = 50 - g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
	}
	else if (g_pfData[9] > 0 && g_pfData[10] > 0) { // both reverse
		PWMPulseWidth_0 = 50 - g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
		PWMPulseWidth_1 = 50 - g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
		PWMPulseWidth_2 = 50 + g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
		PWMPulseWidth_3 = 50 + g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
	}
	else if (g_pfData[9] > 0 && g_pfData[10] < 0) { // roll reverse
		PWMPulseWidth_0 = 50 - g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
		PWMPulseWidth_1 = 50 - g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
		PWMPulseWidth_2 = 50 + g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
		PWMPulseWidth_3 = 50 + g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
	}
	else if (g_pfData[9] < 0 && g_pfData[10] < 0) { // pitch reverse
		PWMPulseWidth_0 = 50 + g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
		PWMPulseWidth_1 = 50 + g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
		PWMPulseWidth_2 = 50 - g_roll_pid.myOutput/2 - g_pitch_pid.myOutput/2;
		PWMPulseWidth_3 = 50 - g_roll_pid.myOutput/2 + g_pitch_pid.myOutput/2;
	}
}
