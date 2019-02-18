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
typedef void (*SamAction_Fct)(Action action);

//Programmed Actions
Action progActions[SIZEOF_PROG_ACTIONS];

//Directly injected Actions
volatile uint8_t write_idx_action;
volatile uint8_t read_idx_action;
Action actionBuffer[SIZEOF_ACTIONBUFFER];

SamValue_Fct valueFcts[255];
SamAction_Fct actionFcts[255];

void rp_am_init();

uint8_t rp_am_registerSAMValueIdentfier(uint8_t samId, SamValue_Fct fct);
uint8_t rp_am_registerSAMActionIdentfier(uint8_t samId, SamAction_Fct fct);

void rp_am_addAction(Action action);
void rp_am_addActionID(uint8_t actionMemID);

void rp_am_triggerAction(Action action);

/*
 * Checks DirectActionQueue
 */
uint8_t rp_am_tick();

#endif /* BLUEHOME_RULEPROCESS_RP_ACTIONMANAGER_H_ */
