/*
 * pid.c
 *
 * Created on: 2015/7/28
 * Author: Bill Yiqiu Wang
 *
 * Insights taken from PID_v1 arduino library - Brett Beauregard
 *
 */



#include "pid.h"
#include "stdbool.h"
#include "control.h"


//*****************************************************************************
// PIDConfig()
//
// This is the PID config function, updates in input, output, setpoint, Kp, Ki, Kd
//*****************************************************************************
void
PIDConfig(float Input, float Output, float Setpoint, float Kp, float Ki, float Kd, int ControllerDirection, pidInstance* pointer)
{

	pointer->myOutput = Output;
	pointer->myInput = Input;
	pointer->mySetpoint = Setpoint;
	pointer->inAuto = true;

	// Reset the limits
	int Min = 0;
	int Max = 255;
	if(Min >= Max) return;
	pointer->outMin = Min;
	pointer->outMax = Max;
	if(pointer->inAuto)
	{
		if(pointer->myOutput > pointer->outMax) pointer->myOutput = pointer->outMax;
		else if(pointer->myOutput < pointer->outMin) pointer->myOutput = pointer->outMin;

		if(pointer->ITerm > pointer->outMax) pointer->ITerm= pointer->outMax;
		else if(pointer->ITerm < pointer->outMin) pointer->ITerm= pointer->outMin;
	}
	// Default Min and Max are 0 and 100

	pointer->SampleTime = 100;
	//default Controller Sample Time is 0.1 seconds

	// SetControllerDirection
	if(pointer->inAuto && ControllerDirection !=pointer->controllerDirection)
	{
		pointer->kp = (0 - pointer->kp);
		pointer->ki = (0 - pointer->ki);
		pointer->kd = (0 - pointer->kd);
	}
	pointer->controllerDirection = ControllerDirection;



	// Reset the tuning
	if (Kp<0 || Ki<0 || Kd<0) return;

	float SampleTimeInSec = ((float)pointer->SampleTime)/1000;
	pointer->kp = Kp;
	pointer->ki = Ki * SampleTimeInSec;
	pointer->kd = Kd / SampleTimeInSec;

	if(pointer->controllerDirection ==REVERSE)
	{
		pointer->kp = (0 - pointer->kp);
		pointer->ki = (0 - pointer->ki);
		pointer->kd = (0 - pointer->kd);
	}

	pointer->lastTime = millis();
}


//*****************************************************************************
// ComputePID()
//
// This is the PID computation function, this function should be put into a while loop
// This function itself will decide whether a output needs to be computed
//*****************************************************************************
bool
ComputePID(pidInstance* pointer)
{

	if(!pointer->inAuto) return false;

	unsigned long now = millis();
	unsigned long timeChange = (now - pointer->lastTime);
	if(timeChange>= pointer->SampleTime)
	{
		/*Compute all the working error variables*/
		float input = pointer->myInput;
		float error = pointer->mySetpoint - input;
		pointer->ITerm+= (pointer->ki * error);
		if(pointer->ITerm > pointer->outMax) pointer->ITerm= pointer->outMax;
		else if(pointer->ITerm < pointer->outMin) pointer->ITerm= pointer->outMin;
		float dInput = (input - pointer->lastInput);

		/*Compute PID Output*/
		float output = pointer->kp * error + pointer->ITerm - pointer->kd * dInput;

		if(output > pointer->outMax) output = pointer->outMax;
		else if(output < pointer->outMin) output = pointer->outMin;
		pointer->myOutput = output;

		/*Remember some variables for next time*/
		pointer->lastInput = input;
		pointer->lastTime = now;
		return true;
	}
	else return false;
}


//*****************************************************************************
// SetParam()
//
// This function changes PID Param is called by the PID init function,
//*****************************************************************************
void
SetParam(float Kp, float Ki, float Kd, pidInstance* pointer)
{
	if (Kp<0 || Ki<0 || Kd<0) return;



	float SampleTimeInSec = ((float)pointer->SampleTime)/1000;
	pointer->kp = Kp;
	pointer->ki = Ki * SampleTimeInSec;
	pointer->kd = Kd / SampleTimeInSec;

	if(pointer->controllerDirection ==REVERSE)
	{
		pointer->kp = (0 - pointer->kp);
		pointer->ki = (0 - pointer->ki);
		pointer->kd = (0 - pointer->kd);
	}
}

//*****************************************************************************
// SetInterval
//
// This function sets the time interval in between ComputePID is called
//*****************************************************************************
void
SetInterval(int NewSampleTime, pidInstance* pointer)
{
	if (NewSampleTime > 0)
	{
		float ratio  = (float)NewSampleTime
				/ (float)pointer->SampleTime;
		pointer->ki *= ratio;
		pointer->kd /= ratio;
		pointer->SampleTime = (unsigned long)NewSampleTime;
	}
}

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
SetOutputLimits(float Min, float Max, pidInstance* pointer)
{
	if(Min >= Max) return;
	pointer->outMin = Min;
	pointer->outMax = Max;

	if(pointer->inAuto)
	{
		if(pointer->myOutput > pointer->outMax) pointer->myOutput = pointer->outMax;
		else if(pointer->myOutput < pointer->outMin) pointer->myOutput = pointer->outMin;

		if(pointer->ITerm > pointer->outMax) pointer->ITerm= pointer->outMax;
		else if(pointer->ITerm < pointer->outMin) pointer->ITerm= pointer->outMin;
	}
}

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
SetMode(int Mode, pidInstance* pointer)
{
	bool newAuto = (Mode == AUTOMATIC);
	if(newAuto == !pointer->inAuto)
	{   // We just went from manual to auto
		// Hence reinitializa the controller
		pointer->ITerm = pointer->myOutput;
		pointer->lastInput = pointer->myInput;
		if(pointer->ITerm > pointer->outMax) pointer->ITerm = pointer->outMax;
		else if(pointer->ITerm < pointer->outMin) pointer->ITerm = pointer->outMin;
	}
	pointer->inAuto = newAuto;
}

//*****************************************************************************
// Initialize()
//
//	This function ensures no bump when switched from manual back to automatic
//
//*****************************************************************************
void
Initialize(pidInstance* pointer)
{

	pointer->ITerm = pointer->myOutput;
	pointer->lastInput = pointer->myInput;
	if(pointer->ITerm > pointer->outMax) pointer->ITerm = pointer->outMax;
	else if(pointer->ITerm < pointer->outMin) pointer->ITerm = pointer->outMin;
}

//*****************************************************************************
// SetControllerDirection()
//
// This function sets the direction of the output, and is called by the PIDConfig
//*****************************************************************************
void
SetControllerDirection(int Direction, pidInstance* pointer)
{
	if(pointer->inAuto && Direction !=pointer->controllerDirection)
	{
		pointer->kp = (0 - pointer->kp);
		pointer->ki = (0 - pointer->ki);
		pointer->kd = (0 - pointer->kd);
	}
	pointer->controllerDirection = Direction;
}

