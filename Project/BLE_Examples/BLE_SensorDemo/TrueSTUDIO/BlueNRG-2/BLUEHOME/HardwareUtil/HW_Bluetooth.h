/*
 * HW_Bluetooth.h
 *
 *  Created on: 13.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_BLUETOOTH_H_
#define BLUEHOME_HARDWAREUTIL_HW_BLUETOOTH_H_


#include "bluenrg1_api.h"
#include <sys/_stdint.h>

uint8_t Sensor_DeviceInit(void);
void APP_Tick(void);

extern uint8_t Application_Max_Attribute_Records[];


//Assert Flag
extern uint8_t BLUETOOTH_ERR_FLAG;

void hw_bl_init();

void hw_bl_setDeviceConnectable(void);

#endif /* BLUEHOME_HARDWAREUTIL_HW_BLUETOOTH_H_ */
