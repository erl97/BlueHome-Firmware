/*
 * SAM_Programming.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "SourceActionManager/SAM_Programming.h"
#include "SourceActionManager/SAM_Init.h"

#include "HardwareUtil/HW_Init.h"
#include "HardwareUtil/HW_MAC.h"

#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_RuleChecker.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"

#include <string.h>

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

	switch(action->actionID){
		case SAM_PROGRAMMING_ACT_ID_WRITE_RULE:{
			db_cs_printString("Programming Rule...\r");

			Rule r;
			r.actionMemID = directActionMemID;
			r.sourceSAM = directSourceSAMID;
			r.sourceID = directSourceID;
			memcpy(r.param, directParam, MAX_PARAM);
			memcpy(r.paramComp, directCompParam, MAX_PARAM);

			rp_rc_updateRules(&r, directRuleMemID);

			break;
		}
		case SAM_PROGRAMMING_ACT_ID_WRITE_ACTION:{
			db_cs_printString("Programming Action...\r");

			Action a;
			a.actionID = directActionID;
			a.actionSAM = directActionSAMID;
			a.paramMask = directMask;
			memcpy(a.param, directParam, MAX_PARAM);

			rp_am_updateAction(&a, directActionMemID);

			break;
		}
		case SAM_PROGRAMMING_ACT_ID_WRITE_MAC:{
			for(int i = 0; i < MAX_PARAM; i += 7){

				uint8_t macID = action->param[i];

				if(macID == 0){
					if(i == 0){
						db_as_assert(DB_AS_ERROR_PROGRAM, "Not allowed to reprogram own MAC !\r");
						return;
					}else return;
				}

				uint8_t mac[6];
				for(int j = 0; j < 6; j++){
					mac[5-j] = action->param[i+1+j];
				}

				db_cs_printString("Programming MAC...\r");
				db_cs_printString("ID: ");
				db_cs_printInt(macID);
				db_cs_printString("\r");
				db_cs_printMAC(mac);
				db_cs_printString("\r");

				//Programming MAC
				hw_mac_updateMac(mac, macID);
			}
			break;
		}
		case SAM_PROGRAMMING_ACT_ID_CLEAR_RULE:{
			db_cs_printString("Clear Rules...\r");
			rp_rc_clearRules();
			break;
		}
		case SAM_PROGRAMMING_ACT_ID_CLEAR_ACTION:{
			db_cs_printString("Clear Actions...\r");
			rp_am_clearActions();
			break;
		}
		case SAM_PROGRAMMING_ACT_ID_CLEAR_MAC:{
			db_cs_printString("Clear Macs...\r");
			void hw_mac_clear();
			break;
		}
	}

}
