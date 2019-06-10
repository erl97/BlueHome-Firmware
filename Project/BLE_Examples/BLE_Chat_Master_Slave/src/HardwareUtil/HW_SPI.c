/*
 * HW_SPI.c
 *
 *  Created on: 10.06.2019
 *      Author: Oliver
 */

#include "HardwareUtil/HW_SPI.h"
#include "HardwareUtil/HW_GPIO.h"

#include "BlueNRG1_spi.h"
#include "BlueNRG2.h"
#include "BlueNRG1_sysCtrl.h"

#include "misc.h"

#include <stdint.h>

#include "Debug/DB_Console.h"

void hw_spi_init(){

	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_SPI, ENABLE);

	hw_gpio_init_PinSerial0(HW_SPI_SCK_PIN);
	hw_gpio_init_PinSerial0(HW_SPI_MOSI_PIN);
	hw_gpio_init_PinSerial0(HW_SPI_CS_PIN);

	db_cs_printString("Init SPI...\r");

	SPI_InitType initStruct = {
			SPI_Mode_Master, //SPI_Mode
			HW_SPI_TRANSMIT_LENGTH, //SPI_DataSize
			SPI_CPOL_Low, //SPI_CPOL
			SPI_CPHA_1Edge, //SPI_CPHA
			1000000 //SPI_BaudRate
	};

	SPI_Init(&initStruct);
	SPI_FrameFormatConfig(SPI_FrmFrmt_Motorola);
	SPI_SetMasterCommunicationMode(SPI_TRANSMIT_MODE);
	SPI_SlaveSwSelection(ENABLE);

	SPI_Cmd(ENABLE);

	/* NVIC configuration */
	NVIC_InitType NVIC_InitStructure;

	//Enable Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = SPI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Test Display

	//Booster soft start
	hw_spi_write(0x0006, 1);
	hw_spi_write(0x0107, 1);
	hw_spi_write(0x0107, 1);
	hw_spi_write(0x0104, 1);

	//Power Optimization ?

	//DFV_EN Reset
	hw_spi_write(0x0016, 1);

	//Power setting
	hw_spi_write(0x0001, 1);
	hw_spi_write(0x0103, 1);
	hw_spi_write(0x0100, 1);
	hw_spi_write(0x012b, 1);
	hw_spi_write(0x012b, 1);
	hw_spi_write(0x0109, 1);

	//Power on
	hw_spi_write(0x0004, 1);

	//Wait
	for(int i = 0; i < 1000; i++){
		db_cs_printString("");
	}

	//Panel setting
	hw_spi_write(0x0000, 1);
	hw_spi_write(0x01af, 1);

	//PLL control
	hw_spi_write(0x0030, 1);
	hw_spi_write(0x013a, 1);

	//Resolution setting
	hw_spi_write(0x0061, 1);

	hw_spi_write(0x0100, 1);
	hw_spi_write(0x01b0, 1);
	hw_spi_write(0x0101, 1);
	hw_spi_write(0x0108, 1);

	// VCM_DC setting
	hw_spi_write(0x0082, 1);
	hw_spi_write(0x0112, 1);

	// VCM and data intervall setting
	hw_spi_write(0x0050, 1);
	hw_spi_write(0x0187, 1);

	//Start data transmittion B/W
	hw_spi_write(0x0010, 1);

	for(int i = 0; i < 264*176; i++){
		hw_spi_write(0x0101, 1);
	}

	//Start data transmittion RED
	hw_spi_write(0x0013, 1);

	for(int i = 0; i < 264*176; i++){
		hw_spi_write(0x010F, 1);
	}

	//Display resfresh
	hw_spi_write(0x0012, 1);

}

void hw_spi_write(const uint32_t data, uint8_t wait){

	SPI_BUSY_FLAG = 1;

	SPI_SetNumFramesToTransmit(1);
	SPI_SendData(data);

	SPI_ITConfig(SPI_IT_TE, ENABLE);
	while(SPI_BUSY_FLAG) {}

}

void hw_spi_isp(){
	SPI_BUSY_FLAG = 0;
	SPI_ITConfig(SPI_IT_TE, DISABLE);
}
