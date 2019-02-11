/*
 * RuleProcess_ActionManager.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_RULEPROCESS_RP_ACTIONMANAGER_H_
#define BLUEHOME_RULEPROCESS_RP_ACTIONMANAGER_H_

#include <stdint.h>

#include "RP_Types.h"
#include "RP_Init.h"

typedef uint8_t (*SamValue_Fct)();

//zu Programmstart leerer actions-Array
Action myActions[SIZEOFMYACTIONS];

SamValue_Fct valueFct[255];

void rp_am_init();

uint8_t rp_am_registerSAMIdentfier(uint8_t samId, SamValue_Fct fct);

#endif /* BLUEHOME_RULEPROCESS_RP_ACTIONMANAGER_H_ */
