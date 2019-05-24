/*
 * SAM_Light.c
 *
 *  Created on: 24.05.2019
 *      Author: kutzol63274
 */



#include "SourceActionManager/SAM_Light.h"

#include "SourceActionManager/SAM_Init.h"
#include "HardwareUtil/HW_Init.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "Debug/DB_Console.h"

void sam_light_init()
{
	SAM_PIN_LIGHT_INT = 0xffff;

	hw_init_registerPinIdentfier(SAM_ID_LIGHT, &SAM_PIN_LIGHT_INT);

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_LIGHT, sam_light_triggerSource);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_LIGHT, sam_light_triggerAction);

	// Register ValueFct

}

void sam_light_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("Light Source\r");
	//Get Data
	//Create Source
	//Add to buffer
}

void sam_light_triggerAction(Action *action)
{
	db_cs_printString("Light Action\r");

}
