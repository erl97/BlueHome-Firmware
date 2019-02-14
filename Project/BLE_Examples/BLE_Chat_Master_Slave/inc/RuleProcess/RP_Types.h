/*
 * RuleProcessTypes.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_RULEPROCESS_RULEPROCESS_TYPES_H_
#define BLUEHOME_RULEPROCESS_RULEPROCESS_TYPES_H_

#include <stdint.h>

////////////////////////////////// SOURCE ////////////////////////////////////////
typedef struct
{
	uint8_t sourceType; //Makros
	uint8_t sourceID;	//ID
	uint8_t paramNum;	//number of parameters
	uint8_t param[24]; //Info und Werte
} Source;

////////////////////////////////// RULE ////////////////////////////////////////
typedef struct
{
	uint8_t actionID;	//auszuführende Action, hinterlegt durch ID/Array-Wert
	uint8_t sourceType;		//Source-Makro
	uint8_t sourceID;		//ID der Source
	uint8_t paramNum;		//number of parameters
	uint8_t param[24];		//Info und Werte
	uint8_t paramComp[24];	//how to compare the values
} Rule;

#define BLOCKSIZE_RULES		(sizeof(Rule)/sizeof(uint8_t))		//52

////////////////////////////////// ACTION ////////////////////////////////////////
#define ACTIONTYPE_NOACTION 0
#define ACTIONTYPE_BLUETOOTH 1
#define ACTIONTYPE_DISPLAY 2
#define ACTIONTYPE_RELAIS 3
#define ACTIONSTYPE_LED 4

typedef struct
{
	uint8_t actionType; //Makros
	uint8_t targetID;	//ID
	uint32_t paramMask; //Mask parameters -> constant or variable
	uint8_t paramNum;	//number of parameters
	uint8_t param[24]; //Info und Werte
} Action;

#define BLOCKSIZE_ACTIONS	(sizeof(Action)/sizeof(uint8_t))	//27

#endif /* BLUEHOME_RULEPROCESS_RULEPROCESS_TYPES_H_ */
