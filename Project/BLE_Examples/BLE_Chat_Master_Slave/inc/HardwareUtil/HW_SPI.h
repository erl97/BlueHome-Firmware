/*
 * HW_SPI.h
 *
 *  Created on: 10.06.2019
 *      Author: Oliver
 */

#ifndef USER_HARDWAREUTIL_HW_SPI_H_
#define USER_HARDWAREUTIL_HW_SPI_H_

#include "BlueNRG1_spi.h"
#include "BlueNRG1_gpio.h"

#define HW_SPI_SCK_PIN	GPIO_Pin_0
#define HW_SPI_MOSI_PIN	GPIO_Pin_2
#define HW_SPI_CS_PIN	GPIO_Pin_1

#define HW_SPI_TRANSMIT_LENGTH 	SPI_DataSize_9b

volatile uint8_t SPI_BUSY_FLAG;

void hw_spi_init();
void hw_spi_write(const uint32_t data, uint8_t wait);

void hw_spi_isp();

#endif /* USER_HARDWAREUTIL_HW_SPI_H_ */
