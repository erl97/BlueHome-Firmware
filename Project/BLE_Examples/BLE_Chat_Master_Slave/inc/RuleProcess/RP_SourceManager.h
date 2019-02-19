/*
 * RP_InterruptManager.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_RULEPROCESS_RP_INTERRUPTMANAGER_H_
#define BLUEHOME_RULEPROCESS_RP_INTERRUPTMANAGER_H_

#include <stdint.h>

#include "SourceActionManager/SAM_Init.h"

typedef void (*SamSource_Fct)(uint8_t paramLen, uint8_t *param);

SamSource_Fct sourceFct[SAM_NUM];

void rp_im_init();

uint8_t rp_sm_registerSAMSourceIdentfier(uint8_t samId, SamSource_Fct fct);

void rp_sm_triggerSource(uint8_t samId, uint8_t paramLen, uint8_t *param);

#endif /* BLUEHOME_RULEPROCESS_RP_INTERRUPTMANAGER_H_ */
