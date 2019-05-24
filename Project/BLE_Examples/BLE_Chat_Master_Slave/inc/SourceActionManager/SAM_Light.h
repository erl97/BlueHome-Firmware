/*
 * SAM_Light.h
 *
 *  Created on: 24.05.2019
 *      Author: kutzol63274
 */

#ifndef USER_SOURCEACTIONMANAGER_SAM_LIGHT_H_
#define USER_SOURCEACTIONMANAGER_SAM_LIGHT_H_

#include <stdint.h>

#include "RuleProcess/RP_Types.h"

#define TSL25911_ADDR 0x29

uint32_t SAM_PIN_LIGHT_INT;

void sam_light_init();

void sam_light_triggerSource(uint8_t paramLen, uint8_t *param);
void sam_light_triggerAction(Action *action);


#endif /* USER_SOURCEACTIONMANAGER_SAM_LIGHT_H_ */
