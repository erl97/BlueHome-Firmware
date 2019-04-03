/*
 * RelaisManager.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_RELAIS_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_RELAIS_H_

#include <stdint.h>

#include "RuleProcess/RP_Types.h"

uint32_t SAM_PIN_RELAIS_0;
uint32_t SAM_PIN_RELAIS_1;
uint32_t SAM_PIN_RELAIS_2;
uint32_t SAM_PIN_RELAIS_3;

#define SAM_RELAY_ACT_ID_TOGGLE 	1
#define SAM_RELAY_ACT_ID_ON 		2
#define SAM_RELAY_ACT_ID_OFF 		3

void sam_relay_init();

void sam_relay_triggerSource(uint8_t paramLen, uint8_t *param);
void sam_relay_triggerAction(Action *action);


#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_RELAIS_H_ */
