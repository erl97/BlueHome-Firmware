/*
 * RuleProcessTypes.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_RULEPROCESS_RULEPROCESS_TYPES_H_
#define BLUEHOME_RULEPROCESS_RULEPROCESS_TYPES_H_

#include <stdint.h>

#include "RuleProcess/RP_Init.h"

////////////////////////////////// SOURCE ////////////////////////////////////////
typedef struct
{
	uint8_t sourceSAM; //Source-Manager
	uint8_t sourceID;  //ID
	uint8_t param[MAX_PARAM]; //Info und Werte
} Source;

////////////////////////////////// RULE ////////////////////////////////////////
typedef struct
{
	uint8_t actionMemID;	//auszuführende Action, hinterlegt durch ID/Array-Wert
	uint8_t sourceSAM;		//Source-Manager
	uint8_t sourceID;		//ID der Source
	uint8_t param[MAX_PARAM];		//Info und Werte
	uint8_t paramComp[MAX_PARAM];	//how to compare the values
} Rule;

#define BLOCKSIZE_RULES		(sizeof(Rule)/sizeof(uint8_t))		//52

////////////////////////////////// ACTION ////////////////////////////////////////

typedef struct
{
	uint8_t actionSAM; //Makros
	uint8_t actionID;	//ID
	uint32_t paramMask; //Mask parameters -> constant or variable
	uint8_t param[MAX_PARAM]; //Info und Werte
} Action;

#define BLOCKSIZE_ACTIONS	(sizeof(Action)/sizeof(uint8_t))	//27

#endif /* BLUEHOME_RULEPROCESS_RULEPROCESS_TYPES_H_ */
