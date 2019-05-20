/*
 * HWManager.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HWMANAGER_H_
#define BLUEHOME_HWMANAGER_H_

#include <stdint.h>

#include "Debug/DB_Assert.h"

#define NUM_LOCAL_PIN	14
#define NUM_EXT_PIN		16
#define NUM_MAX_BUSADDR	40

// HW INFO //
uint8_t HW_ID;
uint8_t HW_VERSION;
uint8_t HW_BUS_DEV_NUM;

uint32_t* setPinVar[255];
uint8_t* setBusAddrVar[NUM_MAX_BUSADDR];

//Assert Flags
extern uint32_t PIN_ERR_FLAG;
extern uint8_t ADDR_ERR_FLAG;

uint32_t PIN_OPEN_FLAG;

void hw_init_init();

uint8_t hw_init_registerPinIdentfier(uint8_t pinID, uint32_t* addr);
uint8_t hw_init_registerBusAddrIdentfier(uint8_t samID, uint8_t* addr);

uint32_t hw_init_getPinFromPinId(uint8_t pinId);
uint8_t hw_init_getSamIdFromPinId(uint8_t pinId);

void hw_init_pins();

#endif /* BLUEHOME_HWMANAGER_H_ */
