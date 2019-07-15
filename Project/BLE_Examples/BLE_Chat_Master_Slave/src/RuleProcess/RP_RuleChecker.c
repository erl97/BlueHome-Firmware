/*
 * source.c
 *
 *  Created on: 08.01.2019
 *      Author: dinkelsv64505
 */

#include "RuleProcess/RP_RuleChecker.h"

#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_Init.h"
#include "RuleProcess/RP_Types.h"

#include "HardwareUtil/HW_Memory.h"

#include "Debug/DB_Assert.h"
#include "Debug/DB_Console.h"

#include "SourceActionManager/SAM_Init.h"

#include "BlueNRG1_flash.h"

#include "bluenrg1_stack.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint8_t updateFlash_Rule = 0;

void rp_rc_loadRules();

void rp_rc_init()
{
	rp_rc_reloadRules();

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

void rp_rc_clearRules()
{
	for(int i = 0; i < SIZEOF_RULES; i++){

		progRules[i].actionMemID = 255;
		progRules[i].sourceSAM = 255;
		progRules[i].sourceID = 255;

		for (int l = 0; l < MAX_PARAM; l++){
			progRules[i].param[l] = 255;
		}

		for (int l = 0; l < MAX_PARAM; l++){
			progRules[i].paramComp[l] = 255;
		}

	}
	updateFlash_Rule = 1;
}

void rp_rc_reloadRules()
{
	uint8_t index = 0;
	// Rule
	for(int i = 0; i < SIZEOF_RULES; i++){
		index = 0;

		progRules[i].actionMemID = FLASH_ReadByte(_MEMORY_RULES_ADDR + (i * BLOCKSIZE_RULES) + index);
		index++;

		progRules[i].sourceSAM = FLASH_ReadByte(_MEMORY_RULES_ADDR + (i * BLOCKSIZE_RULES) + index);
		index++;

		progRules[i].sourceID = FLASH_ReadByte(_MEMORY_RULES_ADDR + (i * BLOCKSIZE_RULES) + index);

		index = 4;
		for (int l = 0; l < MAX_PARAM; l++){
			progRules[i].param[l] = FLASH_ReadByte(_MEMORY_RULES_ADDR + (i * BLOCKSIZE_RULES) + index);
			index++;
		}

		index += 4-index%4;
		for (int l = 0; l < MAX_PARAM; l++){
			progRules[i].paramComp[l] = FLASH_ReadByte(_MEMORY_RULES_ADDR + (i * BLOCKSIZE_RULES) + index);
			index++;
		}

	}

}

void rp_rc_updateRules(Rule* rule, uint8_t id){

	db_cs_printString("Update Rules: MEM_ID: ");
	db_cs_printInt(id);
	db_cs_printString(" ");
	db_cs_printRule(rule);

	progRules[id].actionMemID = rule->actionMemID;
	progRules[id].sourceSAM = rule->sourceSAM;
	progRules[id].sourceID = rule->sourceID;

	memcpy(progRules[id].param, rule->param, MAX_PARAM);
	memcpy(progRules[id].paramComp, rule->paramComp, MAX_PARAM);

	updateFlash_Rule = 1;
}

void rp_rc_writeRuleF(uint8_t id){

	uint32_t temp = 0;
	uint8_t index = 0;

	/* Write Flash */
	temp = 0x0000 | (progRules[id].sourceID << 16) | (progRules[id].sourceSAM << 8) | (progRules[id].actionMemID << 0);
	FLASH_ProgramWord(_MEMORY_RULES_ADDR + (id * BLOCKSIZE_RULES) + index, temp);
	index += 4;

	//PARAM
	for(uint8_t l = 0; l < MAX_PARAM; l += 4){
		temp = 0x0000;
		if(l+3 < MAX_PARAM) temp |= (progRules[id].param[l+3] << 24);
		if(l+2 < MAX_PARAM) temp |= (progRules[id].param[l+2] << 16);
		if(l+1 < MAX_PARAM) temp |= (progRules[id].param[l+1] << 8);
		if(l < MAX_PARAM) temp |= (progRules[id].param[l] << 0);

		FLASH_ProgramWord(_MEMORY_RULES_ADDR + (id * BLOCKSIZE_RULES) + index, temp);
		index += 4;
	}

	//PARAM_COMP
	for(uint8_t l = 0; l < MAX_PARAM; l += 4){
		temp = 0x0000;
		if(l+3 < MAX_PARAM) temp |= (progRules[id].paramComp[l+3] << 24);
		if(l+2 < MAX_PARAM) temp |= (progRules[id].paramComp[l+2] << 16);
		if(l+1 < MAX_PARAM) temp |= (progRules[id].paramComp[l+1] << 8);
		if(l < MAX_PARAM) temp |= (progRules[id].paramComp[l] << 0);

		FLASH_ProgramWord(_MEMORY_RULES_ADDR + (id * BLOCKSIZE_RULES) + index, temp);
		index += 4;
	}

	while(FLASH_GetFlagStatus(Flash_CMDDONE) != SET){};

}

void rp_rc_writeRulesToFlash(){

	db_cs_printString("Write Rules to Flash...\r");

	FLASH_Unlock();

	FLASH_ErasePage(_MEMORY_RULES_PAGE);

	/* Wait for the end of erase operation */
	while(FLASH_GetFlagStatus(Flash_CMDDONE) != SET){};

	db_cs_printString("Rule Memory Page erased\r");

	for(uint8_t i = 0; i < SIZEOF_RULES; i++){
		rp_rc_writeRuleF(i);
	}

	FLASH_Lock();

	db_cs_printString("Done\r");
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


//check source packet with rule
void rp_rc_check(Source *source)
{

	uint8_t paramOk = 0;

	for (uint8_t rules_idx = 0; rules_idx < SIZEOF_RULES; rules_idx++)
	{

		if (progRules[rules_idx].sourceSAM == source->sourceSAM){
			if (progRules[rules_idx].sourceID == source->sourceID){
				db_cs_printString("Check rule (SOURCEID/SAM ok) ");
				db_cs_printInt(rules_idx);
				paramOk = 1;
				for (uint8_t i = 0; i < MAX_PARAM; i++){
					if (progRules[rules_idx].paramComp[i] != COMPARE_NOT_IMPORTANT){
						if (progRules[rules_idx].paramComp[i] == COMPARE_EQUALS){
							if (progRules[rules_idx].param[i] != source->param[i]){
								db_cs_printString(" PARAM ");
								db_cs_printInt(i);
								db_cs_printString(" VALUE ");
								db_cs_printInt(source->param[i]);
								db_cs_printString(" NOT EQUAL ");
								db_cs_printInt(progRules[rules_idx].param[i]);
								db_cs_printString(" !");
								paramOk = 0;
								break;
							}
						}else if (progRules[rules_idx].paramComp[i] == COMPARE_GREATER){
							if (progRules[rules_idx].param[i] >= source->param[i]){
								db_cs_printString(" PARAM ");
								db_cs_printInt(i);
								db_cs_printString(" VALUE ");
								db_cs_printInt(source->param[i]);
								db_cs_printString(" NOT GREATER THAN ");
								db_cs_printInt(progRules[rules_idx].param[i]);
								db_cs_printString(" !");
								paramOk = 0;
								break;
							}
						}else if (progRules[rules_idx].paramComp[i] == COMPARE_SMALLER){
							if (progRules[rules_idx].param[i] <= source->param[i]){
								db_cs_printString(" PARAM ");
								db_cs_printInt(i);
								db_cs_printString(" VALUE ");
								db_cs_printInt(source->param[i]);
								db_cs_printString(" NOT SMALLER THAN ");
								db_cs_printInt(progRules[rules_idx].param[i]);
								db_cs_printString(" !");
								paramOk = 0;
								break;
							}
						}
					}
				}
				if (paramOk == 1){
					db_cs_printString(" True !\r");
					rp_am_addActionID(progRules[rules_idx].actionMemID);
				}else db_cs_printString(" False !\r");
			}
		}
	}
}

