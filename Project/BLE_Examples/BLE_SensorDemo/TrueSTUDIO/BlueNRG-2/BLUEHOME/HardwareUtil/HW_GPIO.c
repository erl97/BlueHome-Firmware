/*
 * GPIO.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HW_GPIO.h"

#include <stdint.h>
#include "BlueNRG1_gpio.h"
#include "BlueNRG1_sysCtrl.h"
/**
 * @brief  Configures Buttons.
 * @button_pin pin address of the button. from hw_init_port
 * @retval None.
 */
void hw_gpio_button(uint32_t button_pin)
{
	GPIO_InitType GPIO_InitStructure;

	/* Enables the BUTTON Clock */
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

	/* Configures Button pin as input */
	GPIO_InitStructure.GPIO_Pin = button_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Input;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);

}