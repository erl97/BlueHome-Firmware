/*
 * RTCManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "SourceActionManager/SAM_RTC.h"

#include "SourceActionManager/SAM_Init.h"
#include "HardwareUtil/HW_Init.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "Debug/DB_Console.h"

void sam_rtc_init()
{
	SAM_PIN_RTC_INT = 0xffff;

	hw_init_registerPinIdentfier(SAM_ID_RTC, &SAM_PIN_RTC_INT);
	hw_init_registerBusAddrIdentfier(SAM_ID_RTC, &SAM_BUS_ADDR_RTC);

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_RTC, sam_rtc_triggerSource);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_RTC, sam_rtc_triggerAction);

	// Register ValueFct

}

void sam_rtc_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("RTC Source\r");
	//Get Data
	//Create Source
	//Add to buffer
}

void sam_rtc_triggerAction(Action *action)
{
	db_cs_printString("RTC Action\r");

}
