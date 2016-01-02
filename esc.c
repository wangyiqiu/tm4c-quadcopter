/*
 * quad_pwm.c
 *
 *  Created on: 2015/8/3
 *      Author: Bill Yiqiu Wang
 *
 *      This file contains PWM set up for ESC
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"


//*****************************************************************************
//
// Config Param
//
//*****************************************************************************
#define PWM_FREQUENCY 50 // 1/50 => T=20ms
#define PULSE_LOW 1060 // pulse width lower bound, in unit of microseconds
#define PULSE_HIGH 1860 // pulse width higher bound, in unit of microseconds

//*****************************************************************************
//
// Variables
//
//*****************************************************************************
volatile uint32_t ui32Load; // PWM load: counter for pwm period, same speed as PWM clock
volatile uint32_t ui32Load_Low_Bound; // Load bounded by PULSE_LOW
volatile uint32_t ui32Load_High_Bound; // Load bounded by PULSE_HIGH
volatile uint32_t ui32Load_Range; // Difference between the above two

// PWM clock
volatile uint32_t ui32PWMClock; // 40M/64, this is FIXED, do NOT try to change this variable, it will NOT work


//*****************************************************************************
//
// Configure M0PWM0 B6
//
//*****************************************************************************
void
M0PWM0_Config()
{
	// GPIO Pin Settings
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);

	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ui32Load);

	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 0);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

//*****************************************************************************
//
// Configure M0PWM1 B7
//
//*****************************************************************************
void
M0PWM1_Config()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// GPIO Pin Settings
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);

	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ui32Load);

	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 0);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}


//*****************************************************************************
//
// Configure M0PWM2 B4
//
//*****************************************************************************
void
M0PWM2_Config()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // pwm module 0 enable
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// GPIO Pin Settings
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
	ROM_GPIOPinConfigure(GPIO_PB4_M0PWM2);

	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ui32Load);

	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
}


//*****************************************************************************
//
// Configure M0PWM3 B5
//
//*****************************************************************************
void
M0PWM3_Config()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // pwm module 0 enable
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// GPIO Pin Settings
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
	ROM_GPIOPinConfigure(GPIO_PB5_M0PWM3);

	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ui32Load);

	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_PWM0);

	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
}

//*****************************************************************************
//
// M0PWM Preconfig
// This function obtains important parameter needed in the pwm setting, must be
// called before all actual pwm config, and after adjusting the Config Param on top
//
//*****************************************************************************
void
ESCConfig() {
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
	ROM_SysCtlPWMClockSet(ui32PWMClock);
	ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_PWM0);

	ui32Load_Low_Bound = PULSE_LOW*0.000001 / ( ( (1)/((double)PWM_FREQUENCY) )/((double)ui32Load) ) +1;
	ui32Load_High_Bound = PULSE_HIGH*0.000001 / ( ( (1)/((double)PWM_FREQUENCY) )/((double)ui32Load) ) -1;
	ui32Load_Range = ui32Load_High_Bound - ui32Load_Low_Bound;

	M0PWM0_Config();
	M0PWM1_Config();
	M0PWM2_Config();
	M0PWM3_Config();
}


//*****************************************************************************
//
// Set speed for ESC, speed range [0,255]
//
//*****************************************************************************
void Set_Speed(uint8_t which, uint8_t speed)
{
	uint32_t pulseWidth = ui32Load_Low_Bound + (speed/(double)256.0) * ui32Load_Range;

	switch(which) {
	case 0:
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, pulseWidth);
		break;
	case 1:
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, pulseWidth);
		break;
	case 2:
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, pulseWidth);
		break;
	case 3:
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, pulseWidth);
		break;
	default:
		break;
	}

}
