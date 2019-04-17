/*
 * I2C.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_I2C_H_
#define BLUEHOME_HARDWAREUTIL_HW_I2C_H_

#include "BlueNRG1_gpio.h"

#define HW_I2C_SCL_PIN	GPIO_Pin_4
#define HW_I2C_SDA_PIN	GPIO_Pin_5

#include <stdint.h>

volatile uint8_t FLAG_BUSY;

void hw_i2c_init();

void hw_i2c_write(uint8_t addr, const uint8_t *data, uint8_t length, uint8_t wait, uint8_t end);
void hw_i2c_isr_bufferEmpty();

#endif /* BLUEHOME_HARDWAREUTIL_HW_I2C_H_ */
