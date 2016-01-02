/*
 * control.h
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
// PWM Pulse Widths Params
//
//*****************************************************************************
extern uint32_t PWMPulseWidth_0;
extern uint32_t PWMPulseWidth_1;
extern uint32_t PWMPulseWidth_2;
extern uint32_t PWMPulseWidth_3;

//*****************************************************************************
//
// The function for starting the "system clock" - WTimer0.
//
// This timer keeps track of the time the entire system has started.
//
//*****************************************************************************
extern void SysTimerStart();


//*****************************************************************************
//
// The function returns the current time in ms
//
//*****************************************************************************
extern unsigned long millis();

//*****************************************************************************
//
// The function to set up the control system
//
//*****************************************************************************
extern void ControlSetup();

//*****************************************************************************
//
// The function to calculate control result
//
//*****************************************************************************
extern void ControlCompute();

#endif /* CONTROL_H_ */
