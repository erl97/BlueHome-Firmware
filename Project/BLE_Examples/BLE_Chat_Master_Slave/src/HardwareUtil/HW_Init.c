/*
 * HWManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include <BlueNRG1_flash.h>
#include <BlueNRG1_gpio.h>
#include <BlueNRG2.h>
#include <Debug/DB_Console.h>
#include <hal_types.h>
#include <HardwareUtil/HW_Bluetooth.h>
#include <HardwareUtil/HW_GPIO.h>
#include <HardwareUtil/HW_I2C.h>
#include <HardwareUtil/HW_Init.h>
#include <HardwareUtil/HW_MAC.h>
#include <HardwareUtil/HW_Memory.h>
#include <HardwareUtil/HW_SPI.h>
#include <SourceActionManager/SAM_Init.h>
#include <misc.h>
#include <sys/_stdint.h>

uint32_t rtc_pin = 0;

// Resolve pinIDs to samIDs
uint8_t pinIdToSam[] = {SAM_ID_UNKNWON, SAM_ID_RELAY, SAM_ID_RELAY, SAM_ID_RELAY, SAM_ID_RELAY, SAM_ID_PIEPER, SAM_ID_TOUCHBUTTON, SAM_ID_LIGHT, SAM_ID_DIO};

void hw_init_init()
{
	// INIT GPIO PIN ASSIGNMENTS
	HW_ID = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_);
	HW_VERSION = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 1);
	HW_BUS_DEV_NUM = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 2);

	db_cs_printString("HW ID: ");
	db_cs_printInt(HW_ID);
	db_cs_printString("\r");

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
	hw_mac_init();
	hw_bl_init();
	hw_i2c_init();
	hw_spi_init();
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

uint32_t hw_init_getPinFromPinId(uint8_t pinID){
	return *setPinVar[pinID];
}

uint8_t hw_init_getSamIdFromPinId(uint8_t pinId){
	return pinIdToSam[pinId];
}

void hw_init_pins()
{

	if (HW_BUS_DEV_NUM > NUM_MAX_BUSADDR)
	{
		ADDR_ERR_FLAG = 255;
		HW_BUS_DEV_NUM = NUM_MAX_BUSADDR;
	}

	// Read Local Pin Config
	db_cs_printString("Pin Config: ");
	uint32_t currPin = GPIO_Pin_0;
	for (int i = 0; i < NUM_LOCAL_PIN; i++)
	{
		uint8_t pinMode = FLASH_ReadByte(_MEMORY_HWCONFIG_BEGIN_ + 16 + i);
		db_cs_printInt(pinMode);

		uint32_t mode = *setPinVar[pinMode];
		*setPinVar[pinMode] = currPin;

		if(mode == 0xffff){ //Configure as output
			hw_gpio_init_PinOut(currPin);
		} else if(mode == 0x0000){ //Configure as input
			hw_gpio_init_PinIn(currPin);
		} else if(mode == 0xfff0){
			hw_gpio_init_PinSerial0(currPin);
		} else if(mode == 0xfff1){
			hw_gpio_init_PinSerial1(currPin);
		} else if(mode == 0xfff2){
			hw_gpio_init_PinSerial2(currPin);
		}

		currPin = currPin << 1;
	}
	db_cs_printString("\r");

	// Disable PIN_1/2 Interrupt
	GPIO_EXTICmd(1, DISABLE);
	GPIO_EXTICmd(2, DISABLE);

	// Enable GPIO Interrupts
	NVIC_InitType NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Read Ext Pin Config (Port Expander)
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

		if(samID < NUM_MAX_BUSADDR){
			*setBusAddrVar[samID] = addr;
		}else{
			CONFIG_ERR_FLAG = 0xff;
			db_as_assert(DB_AS_ERROR_CONFIG, "Missing/Wrong Config !");
		}
	}

}

