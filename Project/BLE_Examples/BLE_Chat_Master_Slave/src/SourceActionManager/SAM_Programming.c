/*
 * SAM_Programming.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "SourceActionManager/SAM_Programming.h"

#include "SourceActionManager/SAM_Init.h"
#include "HardwareUtil/HW_Init.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "Debug/DB_Console.h"

extern uint8_t directParam[MAX_PARAM];
extern uint8_t directCompParam[MAX_PARAM];
extern uint32_t directMask;
extern uint8_t directActionMemID;
extern uint8_t directRuleMemID;
extern uint8_t directActionSAMID;
extern uint8_t directActionID;
extern uint8_t directSourceSAMID;
extern uint8_t directSourceID;

void sam_prog_init()
{

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_PROGRAMMING, sam_prog_triggerSource);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_PROGRAMMING, sam_prog_triggerAction);

	// Register ValueFct

}

void sam_prog_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("Prog Source\r");

}

void sam_prog_triggerAction(Action *action)
{
	db_cs_printString("Prog Action\r");
}
