/*
 * quad_pwm.h
 *
 *  Created on: 2015/8/3
 *      Author: Bill Yiqiu Wang
 */

#ifndef TM4C_QUADCOPTER_QUAD_PWM_H_
#define TM4C_QUADCOPTER_QUAD_PWM_H_

//*****************************************************************************
//
// Set speed for ESC, speed range [0,255]
//
//*****************************************************************************
extern void Set_Speed(uint8_t which, uint8_t speed);

//*****************************************************************************
//
// M0PWM Preconfig
// This function obtains important parameter needed in the pwm setting, must be
// called before all actual pwm config
//
//*****************************************************************************
extern void ESCConfig();


#endif /* TM4C_QUADCOPTER_QUAD_PWM_H_ */
