/*
 * source.c
 *
 *  Created on: 08.01.2019
 *      Author: dinkelsv64505
 */

#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_RuleChecker.h"
#include "RuleProcess/RP_Init.h"
#include "RuleProcess/RP_Types.h"

#include "HardwareUtil/HW_Memory.h"

#include "Debug/DB_Assert.h"
#include "Debug/DB_Console.h"

#include "SourceActionManager/SAM_Init.h"

#include "BlueNRG1_flash.h"

#include <stddef.h>


void rp_rc_loadRules();

void rp_rc_init()
{
	rp_rc_loadRules();

	currentSource = NULL;

	write_idx_source = 0;
	read_idx_source = 0;

	for (int i = 0; i < SIZEOF_SOURCEBUFFER; i++)
	{
		sourceBuffer[i].sourceSAM = SAM_ID_UNKNWON;
	}
}

uint8_t rp_rc_tick(){

	//Check if direct action queue is empty
	if(sourceBuffer[read_idx_source].sourceSAM != SAM_ID_UNKNWON){

		currentSource = &sourceBuffer[read_idx_source];

		//Check for matching rule
		rp_rc_check(currentSource);

		// Delete Action from queue and count up
		sourceBuffer[read_idx_source].sourceSAM = SAM_ID_UNKNWON;
		read_idx_source++;
		if (read_idx_source >= SIZEOF_SOURCEBUFFER)
			read_idx_source = 0;

		return 1;
	}else return 0;
}

void rp_rc_loadRules()
{
	int i;
	i = 0;

	//ein Schleifendurchlauf entspricht einer Rule
	for (int j = 0; i < SIZEOF_RULES; i++, j++)
	{
		progRules[i].actionMemID = FLASH_ReadByte(
		_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + 0);
		progRules[i].sourceSAM = FLASH_ReadByte(
		_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + 1);
		progRules[i].sourceID = FLASH_ReadByte(
		_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + 2);

		for (int k = 4, l = 0; l < MAX_PARAM; k++, l++)
		{
			progRules[i].param[l] = FLASH_ReadByte(
			_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + k);
		}
		for (int k = 28, l = 0; l < MAX_PARAM; k++, l++)
		{
			progRules[i].paramComp[l] = FLASH_ReadByte(
			_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + k);
		}
	}
}

void rp_rc_addSource(Source source)
{
	// Overflow Detection
	if (sourceBuffer[write_idx_source].sourceSAM != SAM_ID_UNKNWON)
	{
		db_as_assert(DB_AS_ERROR_BOVERFLOW, "Source Buffer full !");
	}
	else
	{
		db_cs_printString("Adding Source:\r");
		db_cs_printSource(&source);


		sourceBuffer[write_idx_source] = source;
		write_idx_source++;
		if (write_idx_source >= SIZEOF_SOURCEBUFFER)
			write_idx_source = 0;
	}
}


//Vergleich der source auf Gleichheit mit Inhalt des myRules-Array.
void rp_rc_check(Source *source)
{

	uint8_t i, rules_idx;    //Array-Index auf Rules

	for (rules_idx = 0; rules_idx < SIZEOF_RULES; rules_idx++)
	{
		if (progRules[rules_idx].sourceSAM == source->sourceSAM)
		{
			if (progRules[rules_idx].sourceID == source->sourceID)
			{
				for (i = 0; i < MAX_PARAM; i++)
				{
					if (progRules->paramComp[i] != COMPARE_NOT_IMPORTANT)
					{
						if (progRules->paramComp[i] == COMPARE_EQUALS)
						{
							if (progRules->param[i] != source->param[i])
								break;
						}
						else if (progRules->paramComp[i] == COMPARE_GREATER)
						{
							if (progRules->param[i] <= source->param[i])
								break;
						}
						else if (progRules->paramComp[i] == COMPARE_SMALLER)
						{
							if (progRules->param[i] >= source->param[i])
								break;
						}
					}
				}
				if (i == MAX_PARAM)
				{
					rp_am_addActionID(progRules[rules_idx].actionMemID);
				}
			}
		}
	}
}

