/*
 * SAM_Programming.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_PROGRAMMING_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_PROGRAMMING_H_

#define SAM_PROGRAMMING_ACT_ID_WRITE_RULE 	1
#define SAM_PROGRAMMING_ACT_ID_WRITE_ACTION 2
#define SAM_PROGRAMMING_ACT_ID_WRITE_MAC 	3
#define SAM_PROGRAMMING_ACT_ID_CLEAR_RULE 	4
#define SAM_PROGRAMMING_ACT_ID_CLEAR_ACTION	5
#define SAM_PROGRAMMING_ACT_ID_CLEAR_MAC 	6

#include <stdint.h>

#include "RuleProcess/RP_Types.h"

void sam_prog_init();
void sam_prog_triggerSource(uint8_t paramLen, uint8_t *param);

void sam_prog_triggerAction(Action *action);

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_PROGRAMMING_H_ */
