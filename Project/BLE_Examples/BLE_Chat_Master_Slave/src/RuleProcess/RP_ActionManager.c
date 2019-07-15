/*
 * RuleProcess_ActionManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_Types.h"
#include "RuleProcess/RP_RuleChecker.h"
#include "RuleProcess/RP_Init.h"

#include "BlueNRG1_flash.h"
#include "HardwareUtil/HW_Memory.h"

#include "SourceActionManager/SAM_Init.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"

#include "bluenrg1_stack.h"

#include <stddef.h>
#include <string.h>

uint8_t updateFlash_Action = 0;

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

	rp_am_reloadActions();
}

void rp_am_updateAction(Action* action, uint8_t id){

	db_cs_printString("Update Actions: MEM_ID: ");
	db_cs_printInt(id);
	db_cs_printString(" ");
	db_cs_printAction(action);

	progActions[id].actionID = action->actionID;
	progActions[id].actionSAM = action->actionSAM;
	progActions[id].paramMask = action->paramMask;

	memcpy(progActions[id].param, action->param, MAX_PARAM);

	updateFlash_Action = 1;
}

void rp_am_clearActions()
{

	// Action
	for(int i = 0; i < SIZEOF_PROG_ACTIONS; i++){
		progActions[i].actionSAM = 255;
		progActions[i].actionID = 255;
		progActions[i].paramMask = 0xffff;

		for (int l = 0; l < MAX_PARAM; l++){
			progActions[i].param[l] = 255;
		}
	}

}

void rp_am_reloadActions()
{

	uint8_t index = 0;

	// Action
	for(int i = 0; i < SIZEOF_PROG_ACTIONS; i++){
		index = 0;
		progActions[i].actionSAM = FLASH_ReadByte(_MEMORY_ACTIONS_ADDR + (i * BLOCKSIZE_ACTIONS) + index);
		index++;

		progActions[i].actionID = FLASH_ReadByte(_MEMORY_ACTIONS_ADDR + (i * BLOCKSIZE_ACTIONS) + index);
		index = 4;

		progActions[i].paramMask = FLASH_ReadWord(_MEMORY_ACTIONS_ADDR + (i * BLOCKSIZE_ACTIONS) + index);
		index += 4;

		for (int l = 0; l < MAX_PARAM; l++){
			progActions[i].param[l] = FLASH_ReadByte(_MEMORY_ACTIONS_ADDR + (i * BLOCKSIZE_ACTIONS) + index);
			index++;
		}

	}

}

void rp_am_writeActionF(uint8_t id){

	uint32_t temp = 0;
	uint8_t index = 0;

	/* Write Flash */
	temp = 0x0000 | (progActions[id].actionID << 8) | (progActions[id].actionSAM << 0);
	FLASH_ProgramWord(_MEMORY_ACTIONS_ADDR + (id * BLOCKSIZE_ACTIONS) + index, temp);
	index += 4;

	temp = progActions[id].paramMask;
	FLASH_ProgramWord(_MEMORY_ACTIONS_ADDR + (id * BLOCKSIZE_ACTIONS) + index, temp);
	index += 4;

	//PARAM
	for(uint8_t l = 0; l < MAX_PARAM; l += 4){
		temp = 0x0000;
		if(l+3 < MAX_PARAM) temp |= (progActions[id].param[l+3] << 24);
		if(l+2 < MAX_PARAM) temp |= (progActions[id].param[l+2] << 16);
		if(l+1 < MAX_PARAM) temp |= (progActions[id].param[l+1] << 8);
		if(l < MAX_PARAM) temp |= (progActions[id].param[l] << 0);

		FLASH_ProgramWord(_MEMORY_ACTIONS_ADDR + (id * BLOCKSIZE_ACTIONS) + index, temp);
		index += 4;
	}

	while(FLASH_GetFlagStatus(Flash_CMDDONE) != SET){};

}

void rp_am_writeActionsToFlash(){

	db_cs_printString("Write Actions to Flash...\r");

	FLASH_Unlock();

	FLASH_ErasePage(_MEMORY_ACTIONS_PAGE);

	/* Wait for the end of erase operation */
	while(FLASH_GetFlagStatus(Flash_CMDDONE) != SET){};

	db_cs_printString("Action Memory Page erased\r");

	for(uint8_t i = 0; i < SIZEOF_MAC; i++){
		rp_am_writeActionF(i);
	}

	FLASH_Lock();

	db_cs_printString("Done\r");

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

void rp_am_addAction(Action *action){
	if (actionBuffer[write_idx_action].actionSAM != SAM_ID_UNKNWON)
		{
			db_as_assert(DB_AS_ERROR_BOVERFLOW, "Action Buffer full !");
		}
		else
		{
			db_cs_printString("Adding Action:\r");
			db_cs_printAction(action);

			actionBuffer[write_idx_action] = *action;
			write_idx_action++;
			if (write_idx_action >= SIZEOF_ACTIONBUFFER)
				write_idx_action = 0;
		}
}

void rp_am_addActionID(uint8_t actionMemID){
	rp_am_addAction(&progActions[actionMemID]);
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
	for(uint8_t i = 0; i < MAX_PARAM; i++){
		if(action->paramMask & (1 << i)){
			action->param[i] = valueFcts[i](i);
		}
	}
}


void rp_am_triggerAction(Action *action){
	db_cs_printString("Trigger Action: \r");
	db_cs_printAction(action);

	//Replace Action Masked Fields
//	rp_am_replaceMask(action); TODO MASK !

	//Call Action Function
	if(action->actionSAM < SAM_NUM) actionFcts[action->actionSAM](action);
	else{
		db_as_assert(DB_AS_ERROR_ACTIONFCT, "Action: SAM ID unknown!");
	}

}


