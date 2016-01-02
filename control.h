/*
 * control.h
 *
 *  Created on: Jan 1, 2016
 *      Author: billwang
 */
#include <stdint.h>

#ifndef CONTROL_H_
#define CONTROL_H_

//*****************************************************************************
// global array used to store compdcm data,
// g_pfData[9]	-roll
// g_pfData[10]	-pitch
// g_pfData[11]	-raw
//*****************************************************************************
extern float g_pfData[16];

//*****************************************************************************
//
// ESC Speed Values
//
//*****************************************************************************
extern uint8_t g_ESC0_Speed;
extern uint8_t g_ESC1_Speed;
extern uint8_t g_ESC2_Speed;
extern uint8_t g_ESC3_Speed;

//*****************************************************************************
//
// The function for starting the "system clock" - WTimer0.
//
// This timer keeps track of the time the entire system has started.
//
//*****************************************************************************
extern void SysTimerConfig();


//*****************************************************************************
//
// The function returns the current time in ms
//
//*****************************************************************************
extern unsigned long MilliSecondsTime();

//*****************************************************************************
//
// The function for starting the interrupt - Timer0A.
//
// This timer wakes the cpu up for reading dcm value and control algorithm
//
//*****************************************************************************
extern void WakeAlarmConfig();

//*****************************************************************************
//
// The interrupt handler for the control timer.
//
//*****************************************************************************
void IntWakeUp(void);

#endif /* CONTROL_H_ */
