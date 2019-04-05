/*
 * GPIO.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_GPIO_H_
#define BLUEHOME_HARDWAREUTIL_HW_GPIO_H_

#include <stdint.h>

void hw_gpio_init();

void hw_gpio_init_PinOut(uint32_t pin);
void hw_gpio_init_PinSerial0(uint32_t pin);
void hw_gpio_init_PinSerial1(uint32_t pin);
void hw_gpio_init_PinSerial2(uint32_t pin);

#endif /* BLUEHOME_HARDWAREUTIL_HW_GPIO_H_ */
