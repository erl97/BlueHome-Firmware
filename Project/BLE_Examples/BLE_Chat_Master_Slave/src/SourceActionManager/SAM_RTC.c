/*
 * RTCManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "SourceActionManager/SAM_RTC.h"

#include "SourceActionManager/SAM_Init.h"
#include "HardwareUtil/HW_Init.h"

void sam_rtc_init()
{
	SAM_PIN_RTC_INT = 0xffff;

	hw_init_registerPinIdentfier(SAM_PIN_ID_RTCINT, &SAM_PIN_RTC_INT);
	hw_init_registerBusAddrIdentfier(SAM_DEVICE_ID_RTC, &SAM_BUS_ADDR_RTC);

}

void sam_rtc_notifyEvent()
{
	//Get Data
	//Create Source
	//Add to buffer
}