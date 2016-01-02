/*
 * compdcm_9150.h
 *
 * Created on: 2015/7/26
 * Author: Bill Yiqiu Wang
 */

#include "sensorlib/hw_mpu9150.h"
#include "sensorlib/hw_ak8975.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/ak8975.h"
#include "sensorlib/mpu9150.h"
#include "sensorlib/comp_dcm.h"

#ifndef COMPDCM_9150_H_
#define COMPDCM_9150_H_


//*****************************************************************************
//
// Define MPU9150 I2C Address.
//
//*****************************************************************************
#define MPU9150_I2C_ADDRESS     0x68


//*****************************************************************************
//
// Global instance structure for the I2C master driver.
//
//*****************************************************************************
extern tI2CMInstance g_sI2CInst;

//*****************************************************************************
//
// Global instance structure for the ISL29023 sensor driver.
//
//*****************************************************************************
extern tMPU9150 g_sMPU9150Inst;

//*****************************************************************************
//
// Global Instance structure to manage the DCM state.
//
//*****************************************************************************
extern tCompDCM g_sCompDCMInst;


//*****************************************************************************
//
// Called by the NVIC as a result of GPIO port B interrupt event. For this
// application GPIO port B pin 2 is the interrupt line for the MPU9150
//
//*****************************************************************************
void
IntGPIOb(void);

//*****************************************************************************
//
// Called by the NVIC as a result of I2C2 Interrupt. I2C2 is the I2C connection
// to the MPU9150.
//
//*****************************************************************************
void
MPU9150I2CIntHandler(void);

//*****************************************************************************
//
// MPU9150 Sensor callback function.  Called at the end of MPU9150 sensor
// driver transactions. This is called from I2C interrupt context. Therefore,
// we just set a flag and let main do the bulk of the computations and display.
//
//*****************************************************************************
void
MPU9150AppCallback(void *pvCallbackData, uint_fast8_t ui8Status);


//*****************************************************************************
//
// MPU9150 Application error handler. Show the user if we have encountered an
// I2C error.
//
//*****************************************************************************
void
MPU9150AppErrorHandler(char *pcFilename, uint_fast32_t ui32Line);

//*****************************************************************************
//
// Function to wait for the MPU9150 transactions to complete. Use this to spin
// wait on the I2C bus.
//
//*****************************************************************************
void
MPU9150AppI2CWait(char *pcFilename, uint_fast32_t ui32Line);

//*****************************************************************************
//
// MPU9150 I2C pheripheral set up and sensor/compdcm initialization.
//
// Created by Bill Yiqiu Wang 2015/7/27
//
//*****************************************************************************
void
Compdcm9150Config();

//*****************************************************************************
//
// Takes in data array [16], compute dcm data and update the array
//
// Created by Bill Yiqiu Wang on 26July15
//
//*****************************************************************************
void
COMPDCM(float data[]);


#endif /* COMPDCM_9150_H_ */
