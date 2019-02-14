/*
 * RTCManager.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_RTC_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_RTC_H_

#include <stdint.h>

uint32_t SAM_PIN_RTC_INT;
uint8_t SAM_BUS_ADDR_RTC;

void sam_rtc_init();

void sam_rtc_notifyEvent();

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_RTC_H_ */
