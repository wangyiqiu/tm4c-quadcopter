/*
 * pid.h
 *
 * Created on: 2015/7/28
 * Author: Bill Yiqiu Wang
 */


#include "stdbool.h"

#ifndef PID_H_
#define PID_H_

#define AUTOMATIC	1
#define MANUAL	0
#define DIRECT  0
#define REVERSE  1




//*****************************************************************************
//
// The structure that contains the state of a PID instance.
//
//*****************************************************************************
typedef struct
{

	float kp;                  // * (P)roportional Tuning Parameter
	float ki;                  // * (I)ntegral Tuning Parameter
	float kd;                  // * (D)erivative Tuning Parameter

	int controllerDirection;

	float myInput;              // * Pointers to the Input, Output, and Setpoint variables
	float myOutput;             //  pointer5 This creates a hard link between the variables and the
	float mySetpoint;           //   PID, freeing the user from having to constantly tell us
	//   what these values are.  with pointers we'll just know.

	unsigned long lastTime;
	float ITerm, lastInput;

	unsigned long SampleTime;
	float outMin, outMax;

	bool inAuto;

}
pidInstance;



//*****************************************************************************
// PIDConfig()
//
// This is the PID config function, updates in input, output, setpoint, Kp, Ki, Kd
//*****************************************************************************
void
PIDConfig(float Input, float Output, float Setpoint, float Kp, float Ki, float Kd, int ControllerDirection, pidInstance* pointer);



//*****************************************************************************
// ComputePID()
//
// This is the PID computation function, this function should be put into a while loop
// This function itself will decide whether a output needs to be computed
//*****************************************************************************
bool
ComputePID(pidInstance* pointer);



//*****************************************************************************
// SetParam()
//
// This function changes PID Param is called by the PID init function,
//*****************************************************************************
void
SetParam(float Kp, float Ki, float Kd, pidInstance* pointer);



//*****************************************************************************
// SetInterval
//
// This function sets the time interval in between ComputePID is called
//*****************************************************************************
void
SetInterval(int NewSampleTime, pidInstance* pointer);



//*****************************************************************************
// SetOutputLimits(W)
//
// This function sets the output limit of the PID controlled,
// which can be bound by hardware
//
// Defaut Min Max at config will be 0-100
//
//*****************************************************************************
void
SetOutputLimits(float Min, float Max, pidInstance* pointer);



//*****************************************************************************
// SetMode()
//
// Mode can be
// AUTOMATIC - PID takes care of the output, or
// MANUAL - user takes care of the output
//
// When switched from MANUAL to AUTOMATIC, PID controller will be re-initialized
//*****************************************************************************
void
SetMode(int Mode, pidInstance* pointer);



//*****************************************************************************
// Initialize()
//
//	This function ensures no bump when switched from manual back to automatic
//
//*****************************************************************************
void
Initialize(pidInstance* pointer);



//*****************************************************************************
// SetControllerDirection()
//
// This function sets the direction of the output, and is called by the PIDConfig
//*****************************************************************************
void
SetControllerDirection(int Direction, pidInstance* pointer);





#endif // PID_H_

