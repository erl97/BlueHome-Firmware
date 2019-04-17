/*
 * SAM_TouchButton.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "SourceActionManager/SAM_TouchButton.h"

#include "SourceActionManager/SAM_Init.h"
#include "HardwareUtil/HW_Init.h"

#include "HardwareUtil/HW_I2C.h"

#include "Debug/DB_Console.h"

void sma_tb_programSPM(uint8_t baseAdress, const uint8_t* spmData);

void sam_tb_init()
{

	SAM_PIN_TB_INT = 0xfff0; //Define as GPIOInput

	hw_init_registerPinIdentfier(SAM_PIN_ID_TB_INT, &SAM_PIN_TB_INT);

}

void sam_tb_initHW()
{
	if(SAM_PIN_TB_INT < 0xfff0){
		db_cs_printString("Init TouchButton HW...");

		//hw_i2c_write(uint8_t addr, uint8_t *data, uint8_t length, uint8_t wait, uint8_t end);
		for(int i = 0; i < 16; i++){ //16
			db_cs_printString("Program Touch SPM: ");
			db_cs_printInt(i);
			db_cs_printString("\r");
			sma_tb_programSPM(0x00 + i*8, &initSPMData[i*8]);
		}

	}
}

void sma_tb_programSPM(uint8_t baseAdress, const uint8_t* spmData){
	uint8_t sendData[16];
	sendData[0] = 0x0D; //SpmCfg Register
	sendData[1] = 0x10; //Enable SPM Write
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	sendData[0] = 0x0E; //BaseAddr Register
	sendData[1] = baseAdress;
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	for(int i = 0; i < 8; i++){ //SPM Register
		sendData[i+3] = spmData[i];
	}

	hw_i2c_write(SX8635_ADDR, sendData, 8, 1, 1);


	sendData[0] = 0x0D; //SpmCfg Register
	sendData[1] = 0x00; //Terminate write
	hw_i2c_write(SX8635_ADDR, sendData, 2, 1, 1);

	//for(int i = 0; i < 500000; i++){} //Sleep ~30ms
	for(int i = 0; i < 1000000; i++){} //Sleep ~30ms

}

void sam_tb_notifyEvent()
{

}
