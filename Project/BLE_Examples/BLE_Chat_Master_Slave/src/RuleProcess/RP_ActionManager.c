/*
 * RuleProcess_ActionManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_Types.h"
#include "RuleProcess/RP_RuleChecker.h"

#include "BlueNRG1_flash.h"
#include "HardwareUtil/HW_Memory.h"

#include "SourceActionManager/SAM_Init.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"

#include <stddef.h>

void rp_am_noAction(Action *action){
	db_as_assert(DB_AS_ERROR_ACTIONFCT, " Action Fct not assigned ! ");
}

uint8_t rp_am_noValue(uint8_t samValueID){
	db_as_assert(DB_AS_ERROR_VALUEFCT, " Value Fct not assigned ! ");
	return 0xff;
}

uint8_t rp_am_unknownValue(uint8_t samValueID){
	db_as_assert(DB_AS_ERROR_VALUEFCT, " Asked ValueFct with SAM_ID_UNKOWN ! ");
	return 0xff;
}

uint8_t rp_am_sourceValue(uint8_t samValueID){
	if(samValueID > MAX_PARAM){
		db_as_assert(DB_AS_ERROR_VALUEFCT, " Value Fct - Unknown Source Param ! ");
		return 0xff;
	}else return currentSource->param[samValueID-1];
}

void rp_am_loadActions();

void rp_am_init()
{

	currentAction = NULL;

	write_idx_action = 0;
	read_idx_action = 0;

	for (int i = 0; i < SIZEOF_ACTIONBUFFER; i++)
	{
		actionBuffer[i].actionSAM = SAM_ID_UNKNWON;
	}

	valueFcts[0] = rp_am_unknownValue;
	for (int i = 1; i < 255; i++)
	{
		if(i <= MAX_PARAM) valueFcts[i] = rp_am_sourceValue;
		else valueFcts[i] = rp_am_noValue;
	}

	for (int i = 0; i < SAM_NUM; i++)
	{
		actionFcts[i] = rp_am_noAction;
	}

	rp_am_loadActions();
}

void rp_am_loadActions()
{
	int i = 0;

	//Action
	for (int j = 0; i < SIZEOF_PROG_ACTIONS; i++, j++)
	{
		progActions[i].actionSAM = FLASH_ReadByte(
				_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + 0);
		progActions[i].actionID = FLASH_ReadByte(
				_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + 1);
		progActions[i].paramNum = FLASH_ReadByte(
				_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + 2);

		for (int k = 3, l = 0; l < MAX_PARAM; k++, l++)
		{
			progActions[i].param[l] = FLASH_ReadByte(
					_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + k);
		}
	}
}


//TODO Error Handling
uint8_t rp_am_registerSAMValueIdentfier(uint8_t samId, SamValue_Fct fct)
{
	valueFcts[samId] = fct;
	return 0;
}

uint8_t rp_am_registerSAMActionIdentfier(uint8_t samId, SamAction_Fct fct){
	actionFcts[samId] = fct;
	return 0;
}

void rp_am_addAction(Action action){
	if (actionBuffer[write_idx_action].actionSAM != SAM_ID_UNKNWON)
		{
			db_as_assert(DB_AS_ERROR_BOVERFLOW, "Action Buffer full !");
		}
		else
		{
			db_cs_printString("Adding Action:\r");
			db_cs_printString("SAM: ");
			db_cs_printInt(action.actionSAM);
			db_cs_printString(" ActionID: ");
			db_cs_printInt(action.actionID);
			db_cs_printString("\rParams: ");
			db_cs_printString("[");
			for(int i = 0; i < action.paramNum; i++){
				db_cs_printInt(action.param[i]);
				db_cs_printString(" ");
			}
			db_cs_printString("]\r");

			actionBuffer[write_idx_action] = action;
			write_idx_action++;
			if (write_idx_action >= SIZEOF_ACTIONBUFFER)
				write_idx_action = 0;
		}
}

void rp_am_addActionID(uint8_t actionMemID){
	rp_am_addAction(progActions[actionMemID]);
}


uint8_t rp_am_tick(){

	//Check if direct action queue is empty
	if(actionBuffer[read_idx_action].actionSAM != SAM_ID_UNKNWON){

		currentAction = &actionBuffer[read_idx_action];

		rp_am_triggerAction(currentAction);

		// Delete Action from queue and count up
		actionBuffer[read_idx_action].actionSAM = SAM_ID_UNKNWON;
		read_idx_action++;
		if (read_idx_action >= SIZEOF_ACTIONBUFFER)
			read_idx_action = 0;

		return 1;
	}else return 0;

}

void rp_am_replaceMask(Action *action){
	//Build Action - Replace Mask
	for(uint8_t i = 0; i < action->paramNum; i++){
		if(action->paramMask & (1 << i)){
			action->param[i] = valueFcts[i](i);
		}
	}
}


void rp_am_triggerAction(Action *action){
	db_cs_printString("Trigger Action: \r");
	db_cs_printAction(action);

	//Replace Action Masked Fields
	rp_am_replaceMask(action);

	//Call Action Function
	if(action->actionSAM < SAM_NUM) actionFcts[action->actionSAM](action);
	else{
		db_as_assert(DB_AS_ERROR_ACTIONFCT, "Action: SAM ID unknown!");
	}

}


