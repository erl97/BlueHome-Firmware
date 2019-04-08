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

void hw_i2c_init();

#endif /* BLUEHOME_HARDWAREUTIL_HW_I2C_H_ */
