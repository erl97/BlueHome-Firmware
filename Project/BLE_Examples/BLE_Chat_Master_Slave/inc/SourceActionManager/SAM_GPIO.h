/*
 * SAM_GPIO.h
 *
 *  Created on: 17.04.2019
 *      Author: kutzol63274
 */

#ifndef USER_SOURCEACTIONMANAGER_SAM_GPIO_H_
#define USER_SOURCEACTIONMANAGER_SAM_GPIO_H_

#include <stdint.h>

#include "RuleProcess/RP_Types.h"

uint32_t SAM_PIN_DIO_INT;
uint8_t SAM_BUS_ADDR_GPIO;

#define SAM_GPIO_ACT_ID_TOGGLE 	1
#define SAM_GPIO_ACT_ID_ON 		2
#define SAM_GPIO_ACT_ID_OFF 	3

void sam_gpio_init();
void sam_gpio_initHW();

void sam_gpio_triggerSource(uint8_t paramLen, uint8_t *param);
void sam_gpio_triggerAction(Action *action);

#endif /* USER_SOURCEACTIONMANAGER_SAM_GPIO_H_ */
