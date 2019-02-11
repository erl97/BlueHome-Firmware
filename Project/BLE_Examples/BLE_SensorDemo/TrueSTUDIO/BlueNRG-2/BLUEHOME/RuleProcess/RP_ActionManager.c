/*
 * RuleProcess_ActionManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "RP_ActionManager.h"

#include "BlueNRG1_flash.h"
#include "HardwareUtil/HW_Memory.h"

void rp_am_loadActions();

void rp_am_init()
{
	rp_am_loadActions();
}

void rp_am_loadActions()
{
	int i;
	i = 0;

	//'hardcoding' some actions
	//myActions[i] = {;;;};
	//i++;

	//ein Schleifendurchlauf entspricht einer Action
	for (int j = 0; i < SIZEOFMYACTIONS; i++, j++)
	{
		myActions[i].actionType = FLASH_ReadByte(
				_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + 0);
		myActions[i].targetID = FLASH_ReadByte(
				_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + 1);
		myActions[i].paramNum = FLASH_ReadByte(
				_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + 2);

		for (int k = 3, l = 0; l < 24; k++, l++)
		{
			myActions[i].param[l] = FLASH_ReadByte(
					_MEMORY_ACTIONS_BEGIN_ + (j * BLOCKSIZE_ACTIONS) + k);
		}
	}
}


uint8_t rp_am_registerSAMIdentfier(uint8_t samId, SamValue_Fct fct)
{

	return 0;
}
