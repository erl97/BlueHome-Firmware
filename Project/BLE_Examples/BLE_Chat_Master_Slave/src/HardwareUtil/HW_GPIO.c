/*
 * GPIO.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HardwareUtil/HW_GPIO.h"

#include <stdint.h>
#include "BlueNRG1_gpio.h"
#include "BlueNRG1_sysCtrl.h"
/**
 * @brief  Configures Buttons.
 * @button_pin pin address of the button. from hw_init_port
 * @retval None.
 */
void hw_gpio_init()
{
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

	// TEST CODE BEEPER
//	GPIO_InitType GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Output;
//	GPIO_InitStructure.GPIO_Pull = DISABLE;
//	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//	GPIO_Init(&GPIO_InitStructure);

}

void hw_gpio_init_PinOut(uint32_t pin)
{

	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Output;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);

}
void hw_gpio_init_PinSerial0(uint32_t pin)
{

	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);

}
void hw_gpio_init_PinSerial1(uint32_t pin)
{

	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = Serial1_Mode;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);

}
void hw_gpio_init_PinSerial2(uint32_t pin)
{

	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = Serial2_Mode;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);

}
