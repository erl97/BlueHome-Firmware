/*
 * I2C.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HardwareUtil/HW_I2C.h"

#include "HardwareUtil/HW_GPIO.h"

#include "hal_types.h"
#include "BlueNRG1_i2c.h"
#include "BlueNRG2.h"
#include "BlueNRG1_sysCtrl.h"
#include "misc.h"

#include "Debug/DB_Console.h"

void hw_i2c_init(){

	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_I2C2, ENABLE);

	hw_gpio_init_PinSerial0(HW_I2C_SCL_PIN);
	hw_gpio_init_PinSerial0(HW_I2C_SDA_PIN);

	db_cs_printString("Init I2C...\r");
	I2C_InitType initStruct = {
			100000, //Operation Frequency (Hz)
			I2C_OperatingMode_Master, //Operation Mode (OM)
			I2C_Filtering_Off, //Digital Filter (FON)
			0x0F, //Own Address
			DISABLE //Addressing Mode (SAM) - 7bit
	};

	I2C_Init(I2C2, &initStruct);
	I2C_Cmd(I2C2, ENABLE);

	/* NVIC configuration */
	NVIC_InitType NVIC_InitStructure;

	//Enable Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = I2C2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	I2C_ITConfig(I2C2, I2C_IT_MTD, ENABLE);

	db_cs_printString("Start test transmission...\r");

	I2C_FillTxFIFO(I2C2, 0x00);
	I2C_FillTxFIFO(I2C2, 0xAA);
	I2C_FillTxFIFO(I2C2, 0xAA);
	I2C_FillTxFIFO(I2C2, 0x00);
	I2C_FillTxFIFO(I2C2, 0x00);
	//I2C_FlushTx(I2C2);

	I2C_TransactionType transmissionType;
	transmissionType.Operation = I2C_Operation_Write;
	transmissionType.Address = 0x20;
	transmissionType.AddressType = I2C_AddressType_7Bit;
	transmissionType.StartByte = I2C_StartByte_Disable;
	transmissionType.StopCondition = I2C_StopCondition_Enable;
	transmissionType.Length = 5;

	I2C_BeginTransaction(I2C2, &transmissionType);

	while(1){
		uint32_t val = I2C2->SR_b.STATUS;
		db_cs_printString("Status: ");
		db_cs_printInt(val);
		db_cs_printString("\r");

		val = I2C2->SR_b.CAUSE;
		db_cs_printString("Cause: ");
		db_cs_printInt(val);
		db_cs_printString("\r");
	}


}
