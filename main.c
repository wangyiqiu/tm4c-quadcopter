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


int main(void)
{

    //*****************************************************************************
    //
    // SETUP
    //
    //*****************************************************************************
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Setup the system clock to run at 40 Mhz from PLL with crystal reference
    ESCConfig(); // Config four esc outputs, more in esc.c
	MPU9150Config(); // Initialize/Config accelerometer
	SysTimerConfig(); // Start system timer, which is used to track time, call MilliSecondsTime() to see current time since start in milliseconds

	/*
	 * Not needed in this stage
	 *
	 * WakeAlarm wakes system up through interrupts, optional, recommended when system is put to sleep to save power
	 */
	// WakeAlarmConfig();

    //*****************************************************************************
    //
    // MAIN LOOP
    //
    //*****************************************************************************
    while(1){

    		/*
    		 * Not needed in this stage
    		 *
    		 * It is optional to put system to sleep when it's not calculating anything
    		 * When system is put to sleep, WakeAlarm needs to be configured to wake system up before calculations
    		 * More details could be found in control.c
    		 */
    		// ROM_SysCtlSleep();


    		CompDCM(g_pfData); // update global MPU sensor data in g_pfData

    }
}
