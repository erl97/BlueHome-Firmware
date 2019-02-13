/*
 * source.c
 *
 *  Created on: 08.01.2019
 *      Author: dinkelsv64505
 */
#include "BlueNRG1_flash.h"

#include "RP_RuleChecker.h"
#include "HardwareUtil/HW_Memory.h"

#include "Debug/DB_Assert.h"

void rp_rc_loadRules();

void rp_rc_init()
{
	rp_rc_loadRules();

	write_idx_source = 0;
	read_idx_source = 0;

	for (int i = 0; i < SIZEOFSOURCEBUFFER; i++)
	{
		sourceBuffer[i].sourceType = SOURCETYPE_NOSOURCE;
	}
}

void rp_rc_loadRules()
{
	int i;
	i = 0;

	//'hardcoding' some rules
	//myRules[i] = {;;;};
	//i++;

	//ein Schleifendurchlauf entspricht einer Rule
	for (int j = 0; i < SIZEOFMYRULES; i++, j++)
	{
		myRules[i].actionID = FLASH_ReadByte(
		_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + 0);
		myRules[i].sourceType = FLASH_ReadByte(
		_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + 1);
		myRules[i].sourceID = FLASH_ReadByte(
		_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + 2);
		myRules[i].paramNum = FLASH_ReadByte(
		_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + 3);

		for (int k = 4, l = 0; l < 24; k++, l++)
		{
			myRules[i].param[l] = FLASH_ReadByte(
			_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + k);
		}
		for (int k = 28, l = 0; l < 24; k++, l++)
		{
			myRules[i].paramComp[l] = FLASH_ReadByte(
			_MEMORY_RULES_BEGIN_ + (j * BLOCKSIZE_RULES) + k);
		}
	}
}

void rp_rc_addSource(Source source)
{
	// Overflow Detection
	if (sourceBuffer[write_idx_source].sourceType != SOURCETYPE_NOSOURCE)
	{
		db_as_assert(DB_AS_ERROR_BOVERFLOW, "Source Buffer full !");
	}
	else
	{
		sourceBuffer[write_idx_source] = source;
		write_idx_source++;
		if (write_idx_source >= SIZEOFSOURCEBUFFER)
			write_idx_source = 0;
	}
}


//Vergleich der source auf Gleichheit mit Inhalt des myRules-Array.
void source_handler(Source source)
{
//	uint8_t i; //�nderbar, innere schleife
//	uint8_t rules_idx;    //Array-Index auf Rules
//	for (rules_idx = 0; rules_idx < SIZEOFMYRULES; rules_idx++)
//	{
//		if (myRules[rules_idx].sourceType == source.sourceType)
//		{
//			if (myRules[rules_idx].sourceID == source.sourceID)
//			{
//				for (i = 0; i < myRules->paramNum; i++)
//				{
//					if (myRules->paramComp[i] == COMPARE_NOT_IMPORTANT)
//					{
//						//do nothing
//					}
//					else if (myRules->paramComp[i] == COMPARE_EQUALS)
//					{
//						if (myRules->param[i] == source.param[i])
//						{
//							//true, do nothing
//						}
//						else
//							break;
//					}
//					else if (myRules->paramComp[i] == COMPARE_GREATER)
//					{
//						if (myRules->param[i] > source.param[i])
//						{
//							//true, do nothing
//						}
//						else
//							break;
//					}
//					else if (myRules->paramComp[i] == COMPARE_SMALLER)
//					{
//						if (myRules->param[i] < source.param[i])
//						{
//							//true, do nothing
//						}
//						else
//							break;
//					}
//				}
//				if (i == myRules->paramNum)
//				{
//					//write actionID in actionBuffer
//					actionBuffer[write_idx_action] =
//							myRules[rules_idx].actionID;
//					write_idx_action++;
//					if (write_idx_action == SIZEOFACTIONBUFFER)
//						write_idx_action = 0;
//					break;
//				}
//			}
//		}
//	}
}

