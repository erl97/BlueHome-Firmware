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

uint16_t chatServHandle, TXCharHandle, RXCharHandle;

/* UUIDs */
Service_UUID_t service_uuid;
Char_UUID_t char_uuid;

//Assert Flag
extern uint8_t BLUETOOTH_ERR_FLAG;

void hw_bl_init();

void hw_bl_SetDeviceConnectable(void);


#endif /* BLUEHOME_HARDWAREUTIL_HW_BLUETOOTH_H_ */
