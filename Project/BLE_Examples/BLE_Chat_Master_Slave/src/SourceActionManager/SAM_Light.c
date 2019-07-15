/*
 * SAM_Light.c
 *
 *  Created on: 24.05.2019
 *      Author: kutzol63274
 */



#include "SourceActionManager/SAM_Light.h"
#include "SourceActionManager/SAM_Init.h"

#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_RuleChecker.h"

#include "HardwareUtil/HW_Init.h"
#include "HardwareUtil/HW_I2C.h"


#include "Debug/DB_Console.h"

void sam_light_init()
{
	SAM_PIN_LIGHT_INT = 0x0000;

	hw_init_registerPinIdentfier(SAM_PIN_ID_LIGHT_INT, &SAM_PIN_LIGHT_INT);

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_LIGHT, sam_light_triggerSource);

	// Register ValueFct

}

void sam_light_initHW()
{
	if(SAM_PIN_LIGHT_INT != 0x0000){

		db_cs_printString("HW-Config Light Sensor\r");

		uint8_t sendData[16];
		sendData[0] = 0xA0; //CMD Register
		sendData[1] = 0x13; //Enable Register

		db_cs_printString("Enable Interrupts, turn on ALS\r");
		hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

		sendData[0] = 0xAC; //CMD Register
		sendData[1] = 0x01; //PERSIST Register

		db_cs_printString("Write Persist Register\r");
		hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);


		//Read Current light value
		db_cs_printString("Read current light values\r");
		uint8_t recvData[4];

		hw_i2c_read(TSL25911_ADDR, 0xB4, 1, recvData); 	 // Read CH0 Low Byte
		hw_i2c_read(TSL25911_ADDR, 0xB5, 1, recvData+1); // Read CH0 High Byte
		hw_i2c_read(TSL25911_ADDR, 0xB6, 1, recvData+2); // Read CH1 Low Byte
		hw_i2c_read(TSL25911_ADDR, 0xB7, 1, recvData+3); // Read CH1 High Byte

		for(int i = 0; i < 4; i++){
			db_cs_printInt(recvData[i]);
			db_cs_printString(" ");
		}
		db_cs_printString("\r");


		//Set thresholds
		db_cs_printString("Write Threshold Register\r");

		sendData[0] = 0xA4; //CMD Register
		sendData[1] = recvData[0] < SAM_LIGHT_TH ? 0 : recvData[0] - SAM_LIGHT_TH; //ALS low th low byte Register
		hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

		sendData[0] = 0xA5; //CMD Register
		sendData[1] = recvData[1]; //ALS low th high byte Register
		hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

		sendData[0] = 0xA6; //CMD Register
		sendData[1] = 0xff-recvData[0] < SAM_LIGHT_TH ? 0xff : recvData[0] + SAM_LIGHT_TH; //ALS high th low byte Register
		hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

		sendData[0] = 0xA7; //CMD Register
		sendData[1] = recvData[1]; //ALS high th high byte Register
		hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);


		//Reset Interrupts
		db_cs_printString("Reseting interrupts\r");

		sendData[0] = 0xE7; //CMD Register
		hw_i2c_write(TSL25911_ADDR, sendData, 1, 1, 1);

		//Trigger Int - TEST
//		sendData[0] = 0xE4; //CMD Register
//		hw_i2c_write(TSL25911_ADDR, sendData, 1, 1, 1);

	}
}

void sam_light_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("Light Source\r");

	//Get Data
	//Read Current light value
	db_cs_printString("Read current light values\r");
	uint8_t recvData[4];
	hw_i2c_read(TSL25911_ADDR, 0xB4, 1, recvData); // Read CH0 Low Byte
	hw_i2c_read(TSL25911_ADDR, 0xB5, 1, recvData+1); // Read CH0 High Byte
	hw_i2c_read(TSL25911_ADDR, 0xB6, 1, recvData+2); // Read CH1 Low Byte
	hw_i2c_read(TSL25911_ADDR, 0xB7, 1, recvData+3); // Read CH1 High Byte

	//Create Source
	Source s;

	for(int i = 0; i < 4; i++){
		db_cs_printInt(recvData[i]);
		db_cs_printString(" ");
		s.param[i] = recvData[i];
	}
	db_cs_printString("\r");

	for(int i = 4; i < MAX_PARAM; i++){
		s.param[i] = 0;
	}

	s.sourceSAM = SAM_ID_LIGHT;
	s.sourceID = ACTION_ID_THRESHOLD;

	//Set thresholds
	db_cs_printString("Write Threshold Register\r");

	uint8_t sendData[16];
	sendData[0] = 0xA4; //CMD Register
	sendData[1] = recvData[0] < SAM_LIGHT_TH ? 0 : recvData[0] - SAM_LIGHT_TH; //ALS low th low byte Register
	hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

	sendData[0] = 0xA5; //CMD Register
	sendData[1] = recvData[1]; //ALS low th high byte Register
	hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

	sendData[0] = 0xA6; //CMD Register
	sendData[1] = 0xff-recvData[0] < SAM_LIGHT_TH ? 0xff : recvData[0] + SAM_LIGHT_TH; //ALS high th low byte Register
	hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

	sendData[0] = 0xA7; //CMD Register
	sendData[1] = recvData[1]; //ALS high th high byte Register
	hw_i2c_write(TSL25911_ADDR, sendData, 2, 1, 1);

	db_cs_printString("Reseting interrupts\r");

	sendData[0] = 0xE7; //CMD Register
	hw_i2c_write(TSL25911_ADDR, sendData, 1, 1, 1);

	//Add to buffer
	rp_rc_addSource(s);
}

