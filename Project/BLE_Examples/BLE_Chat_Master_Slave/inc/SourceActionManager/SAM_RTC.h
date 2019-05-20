/*
 * RTCManager.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_RTC_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_RTC_H_

#include <stdint.h>

#include "RuleProcess/RP_Types.h"

uint32_t SAM_PIN_RTC_INT;
uint8_t SAM_BUS_ADDR_RTC;

void sam_rtc_init();

void sam_rtc_triggerSource(uint8_t paramLen, uint8_t *param);
void sam_rtc_triggerAction(Action *action);

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_RTC_H_ */
