/*
 * PWMManager.h
 *
 *  Created on: 05.04.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_PIEPER_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_PIEPER_H_

#include <stdint.h>

#include "RuleProcess/RP_Types.h"

uint32_t SAM_PIN_PIEPER_0;

#define SAM_PIEPER_ACT_ID_PEEP		1
#define SAM_PIEPER_ACT_ID_CONSTANT	2

void sam_pieper_init();
void sam_pieper_initHW();

void sam_pieper_triggerAction(Action *action);

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_Pieper_H_ */
