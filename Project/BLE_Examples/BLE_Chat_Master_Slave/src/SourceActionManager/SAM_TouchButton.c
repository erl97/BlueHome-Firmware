/*
 * SAM_TouchButton.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "SourceActionManager/SAM_TouchButton.h"

#include <stdint.h>

#include "SourceActionManager/SAM_Init.h"

#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "HardwareUtil/HW_Init.h"
#include "HardwareUtil/HW_I2C.h"

#include "RuleProcess/RP_Types.h"
#include "Debug/DB_Console.h"

//0x0E, 0xFA, 0x92, 0x10,

static const uint8_t initSPMData[] = {
		0x00, 0x00, 0x19, 0x00, 0x2B, 0x02, 0x0D, 0x00,
		0x00,
		0x01, //CapModeMisc
		0x17, 0xFF, 0xD5, //CapMode
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CapSensitivity
		0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, //CapThresh
		0x00, //CapPerComp

		0x00, 0x30, 0x50, 0x50, 0x01, 0x0A, 0x00, 0x00,
		0x00, 0x03, 0xff, 0x01, 0x40, 0x50, 0x50, 0x01,
		0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x0F, 0xEA, 0x92, 0x10, //MapAutoLight 0-3
		0x00, 0x00, 0x00, 0x00, 0x02,

		0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44,

		0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50,
		0x46, //CapProxEnable
		0x10, 0x45, 0x02, 0xff, 0xff, 0xff, 0xd5,
		0x55, 0x55, 0x7f, 0x23, 0x22, 0x41, 0xff, 0x74
};

extern volatile uint8_t PROGRAM_FLAG;

void sam_tb_programSPM(uint8_t baseAdress, const uint8_t* spmData);
void sma_tb_readSPM(uint8_t baseAdress, uint8_t* dataBuffer);

void sam_tb_init()
{

	SAM_PIN_TB_INT = 0x0000; //Define as GPIOInput

	hw_init_registerPinIdentfier(SAM_PIN_ID_TB_INT, &SAM_PIN_TB_INT);

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_TOUCHBUTTON, sam_tb_triggerSource);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_TOUCHBUTTON, sam_tb_triggerAction);

}

void sam_tb_initHW()
{
	if(SAM_PIN_TB_INT != 0x0000){
		db_cs_printString("Init TouchButton HW...\r");

		//hw_i2c_write(uint8_t addr, uint8_t *data, uint8_t length, uint8_t wait, uint8_t end);
		//int i = 1;
		for(int i = 0; i < 16; i++){ //16
			db_cs_printString("Program Touch SPM: ");
			db_cs_printInt(i);
			db_cs_printString("\r");
			sam_tb_programSPM(i*8, &initSPMData[i*8]);
		}

		uint8_t data[8];
		for(int i = 0; i < 16; i++){ //16
			db_cs_printString("Read Addr ");
			db_cs_printInt(i);
			db_cs_printString("\r");
			sma_tb_readSPM(i*8, data);
		}

//		while(1){
//			db_cs_printInt(GPIO->DATA);
//			db_cs_printString("\r");
//			for(int i = 0; i < 10000; i++){
//				for(int i = 0; i < 100; i++){
//
//				}
//			}
//		}

	}
}

void sam_tb_programSPM(uint8_t baseAdress, const uint8_t* spmData){

	uint8_t sendData[16];
	sendData[0] = 0x0D; //SpmCfg Register
	sendData[1] = 0x10; //Enable SPM Write

	db_cs_printString("SpmCfg write");
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	sendData[0] = 0x0E; //BaseAddr Register
	sendData[1] = baseAdress;

	db_cs_printString("BaseAddr write");
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	sendData[0] = 0x00; //BaseAddr SPM Write Register

	for(int i = 0; i < 8; i++){ //SPM Register
		sendData[i+1] = spmData[i];
	}

	db_cs_printString("Write SPM");
	hw_i2c_write(SX8635_ADDR, sendData, 9, 1, 1);


	sendData[0] = 0x0D; //SpmCfg Register
	sendData[1] = 0x00; //Terminate write

	db_cs_printString("Reset SpmCfg");
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	db_cs_printString("WAIT");
	PROGRAM_FLAG = 1;
	while(PROGRAM_FLAG){}

	uint8_t recvData[1];
	hw_i2c_read(SX8635_ADDR, 0x00, 1, recvData); // Read SPM Interrupt

}

void sma_tb_readSPM(uint8_t baseAdress, uint8_t* dataBuffer){
	uint8_t sendData[16];
	sendData[0] = 0x0D; //SpmCfg Register
	sendData[1] = 0x18; //Enable SPM Write

	db_cs_printString("SpmCfg write");
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	sendData[0] = 0x0E; //BaseAddr Register
	sendData[1] = baseAdress;

	db_cs_printString("BaseAddr write");
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	db_cs_printString("Read SPM ");
	uint8_t recvData[8];
	hw_i2c_read(SX8635_ADDR, 0x00, 8, recvData);

	for(int i = 0; i < 8; i++){
		db_cs_printInt(recvData[i]);
		db_cs_printString(" ");
	}

	sendData[0] = 0x0D; //SpmCfg Register
	sendData[1] = 0x00; //Terminate write

	db_cs_printString(" Reset SpmCfg\r");
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);
}

void sam_tb_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("Touch Button Source\r");
	db_cs_printString("Cause: ");

	//Get Data
	uint8_t data[1];
	hw_i2c_read(SX8635_ADDR, 0x00, 1, data); // Read Interrupt Source
	db_cs_printInt(data[0]);
	db_cs_printString(" Value: ");

	hw_i2c_read(SX8635_ADDR, 0x01, 1, data); // Read Button Stat (Msb)
	db_cs_printInt(data[0]);
	db_cs_printString(" ");

	hw_i2c_read(SX8635_ADDR, 0x02, 1, data); // Read Button Stat (Lsb)
	db_cs_printInt(data[0]);
	db_cs_printString(" Wheel: ");

	hw_i2c_read(SX8635_ADDR, 0x03, 1, data); // Read Wheel (Msb)
	db_cs_printInt(data[0]);
	db_cs_printString(" ");

	hw_i2c_read(SX8635_ADDR, 0x04, 1, data); // Read Wheel (Lsb)
	db_cs_printInt(data[0]);
	db_cs_printString("\r");

	//Create Source
	//Add to buffer


}

void sam_tb_triggerAction(Action *action)
{
	db_cs_printString("Touch Button Action\r");

}
