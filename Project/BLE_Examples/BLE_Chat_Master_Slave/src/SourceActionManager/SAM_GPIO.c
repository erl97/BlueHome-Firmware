/*
 * SAM_GPIO.c
 *
 *  Created on: 17.04.2019
 *      Author: kutzol63274
 */

#include <Debug/DB_Console.h>
#include <HardwareUtil/HW_Init.h>
#include <HardwareUtil/HW_I2C.h>
#include <RuleProcess/RP_ActionManager.h>
#include <RuleProcess/RP_RuleChecker.h>
#include <RuleProcess/RP_SourceManager.h>
#include <stdint.h>
#include <SourceActionManager/SAM_GPIO.h>
#include <SourceActionManager/SAM_Init.h>

void sam_gpio_init()
{
	SAM_PIN_DIO_INT = 0x0000;
	SAM_BUS_ADDR_GPIO = 0x20;

	hw_init_registerPinIdentfier(SAM_PIN_ID_IO_INT, &SAM_PIN_DIO_INT);
//	hw_init_registerBusAddrIdentfier(SAM_PIN_ID_IO_INT, &SAM_BUS_ADDR_RTC);

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_DIO, sam_gpio_triggerSource);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_DIO, sam_gpio_triggerAction);

	// Register ValueFct

}

void sam_gpio_initHW()
{
	if(SAM_PIN_DIO_INT != 0x0000){

		//IO CONFIG
		uint8_t dataConf[] = {
				0x00, //IO Base Address
				0xF0, //RegDataB
				0xF0, //RegDataA
				0xF0, //RegDirB
				0xF0, //RegDirA
				0x00, //RegPullUpB
				0x00  //RegPullUpA
		};

		hw_i2c_write(SAM_BUS_ADDR_GPIO, dataConf, 7, 1, 1);

		//IRQ CONFIG
		uint8_t dataIRQ[] = {
				0x08, //IRQ Base Address
				0xF0, //IRQMaskB
				0xF0, //IRQMaskA
				0xFF, //IRQEdgeHighB
				0x00, //IRQEdgeHighA
				0xFF, //IRQEdgeLowB
				0x00  //IRQEdgeLowA
		};

		hw_i2c_write(SAM_BUS_ADDR_GPIO, dataIRQ, 7, 1, 1);

		uint8_t dataIC[] = {
				0x0E, //IC Base Address
				0xFF,
				0xFF,
				0xFF,
				0xFF
		};

		hw_i2c_write(SAM_BUS_ADDR_GPIO, dataIC, 5, 1, 1);

	}
}

void sam_gpio_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("GPIO Source\r");

	//Get Data
	uint8_t data[2];
	hw_i2c_read(SAM_BUS_ADDR_GPIO, 0x00, 2, data);


	//Create Source
	Source gpioSource;
	gpioSource.sourceSAM = SAM_ID_DIO;
	gpioSource.sourceID = 1; //RISING_EDGE_CHANGE
//	gpioSource.sourceID = 2; //FALLING_EDGE_CHANGE
	gpioSource.param[0] = data[0];
	gpioSource.param[1] = data[1];

	uint8_t dataIC[] = {
			0x0E, //IC Base Address
			0xFF,
			0xFF,
			0xFF,
			0xFF
	};

	hw_i2c_write(SAM_BUS_ADDR_GPIO, dataIC, 5, 1, 1);

	//Add to buffer
	rp_rc_addSource(gpioSource);
}

void sam_gpio_triggerAction(Action *action)
{
	db_cs_printString("GPIO Action\r");

	uint8_t inData[2];
	hw_i2c_read(SAM_BUS_ADDR_GPIO, 0x00, 2, inData);

	uint8_t data[3];
	data[0] = 0x00; //IO Base Address
	data[1] = inData[0];
	data[2] = inData[1];

	db_cs_printInt(inData[0]);
	db_cs_printString(" ");
	db_cs_printInt(inData[1]);
	db_cs_printString("\r");

	if(action->actionID == SAM_GPIO_ACT_ID_TOGGLE){


		if(action->param[0] >= 4) data[1] ^= (1 << (action->param[0] - 4));
		else data[2] ^= (1 << action->param[0]);

	}else if(action->actionID == SAM_GPIO_ACT_ID_ON){

		if(action->param[0] >= 4) data[1] |= (1 << (action->param[0] - 4));
		else data[2] |= (1 << action->param[0]);

	}else if(action->actionID == SAM_GPIO_ACT_ID_OFF){

		if(action->param[0] >= 4) data[1] &= ~(1 << (action->param[0] - 4));
		else data[2] &= ~(1 << action->param[0]);

	}else if(action->actionID == SAM_GPIO_ACT_ID_ALLOFF){
		data[1] = 0;
		data[2] = 0;
	}

	hw_i2c_write(SAM_BUS_ADDR_GPIO, data, 3, 1, 1);


}
