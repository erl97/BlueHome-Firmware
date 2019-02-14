/*
 * HWManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HardwareUtil/HW_Init.h"


#include <stdint.h>
#include "BlueNRG1_gpio.h"
#include "BlueNRG1_flash.h"

#include "HardwareUtil/HW_Memory.h"
#include "HardwareUtil/HW_Bluetooth.h"

uint32_t rtc_pin = 0;

void hw_init_init()
{
	// INIT GPIO PIN ASSIGNMENTS
	HW_ID = 0x00;
	HW_VERSION = 0x00;
	HW_BUS_DEV_NUM = 0x00;

	setPinVar[0] = &PIN_OPEN_FLAG;
	for (int i = 1; i < sizeof(setPinVar) / sizeof(uint32_t*); i++)
	{
		setPinVar[i] = &PIN_ERR_FLAG;

	}
	for (int i = 0; i < sizeof(setBusAddrVar) / sizeof(uint8_t*); i++)
	{
		setBusAddrVar[i] = &ADDR_ERR_FLAG;
	}

	// INIT HARDWARE MODULES
	hw_bl_init();
}

//TODO: Error Check
uint8_t hw_init_registerPinIdentfier(uint8_t pinID, uint32_t *value)
{
	setPinVar[pinID] = value;
	return 0;
}

uint8_t hw_init_registerBusAddrIdentfier(uint8_t samID, uint8_t* addr)
{
	setBusAddrVar[samID] = addr;
	return 0;
}

//TODO INIT GPIO PINS
void hw_init_gpio()
{

	// Read HW Info
	HW_ID = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_);
	HW_VERSION = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 1);
	HW_BUS_DEV_NUM = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 2);

	if (HW_BUS_DEV_NUM > NUM_MAX_BUSADDR)
	{
		ADDR_ERR_FLAG = 255;
		HW_BUS_DEV_NUM = NUM_MAX_BUSADDR;
	}

	// Read Local Pin Config
	uint32_t currPin = GPIO_Pin_0;
	for (int i = 0; i < NUM_LOCAL_PIN; i++)
	{
		uint8_t pinMode = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 16 + i);
		*setPinVar[pinMode] = currPin;
		currPin = currPin << 1;
	}

	// Read Ext Pin Config
	currPin = NUM_LOCAL_PIN;
	for (int i = 0; i < NUM_EXT_PIN; i++)
	{
		uint8_t pinMode = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 32 + i);
		*setPinVar[pinMode] = currPin;
		currPin++;
	}

	// Read I2C Addr
	for (int i = 0; i < HW_BUS_DEV_NUM; i++)
	{
		uint8_t samID = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 48 + i * 2);
		uint8_t addr = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 48 + 1 + i * 2);

		*setBusAddrVar[samID] = addr;
	}

}

