/*
 * DB_Console.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "Debug/DB_Console.h"

#include "string.h"
#include <stdint.h>
#include <stdlib.h>

#include "Main/BlueHomeApp.h"

#include "HardwareUtil/HW_UART.h"
#include "HardwareUtil/HW_MAC.h"

#include "RuleProcess/RP_Init.h"
#include "RuleProcess/RP_Types.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_RuleChecker.h"

#include "SourceActionManager/SAM_Bluetooth.h"
#include "SourceActionManager/SAM_Programming.h"
#include "SourceActionManager/SAM_Init.h"

#include "Bluetooth/BL_gatt_db.h"
#include "HardwareUtil/HW_Bluetooth.h"

#include "HardwareUtil/HW_Memory.h"


void db_cs_executeCommand(char *cmd)
{
	char *args = strtok(cmd, " ");

	if (strcmp(args, DB_CS_CMD_GETVERSION) == 0){
		db_cs_printString("BlueHome Fimware Version ");
		db_cs_printString(BH_FIRMWARE_VERISON);
		db_cs_printString("\r");

	}else if(strcmp(args, DB_CS_CMD_GETHELP) == 0){
		db_cs_printString(DB_CS_CMD_HELP);

	}else if (strcmp(args, DB_CS_CMD_SENDTESTBL) == 0){
		Action action;
		action.actionSAM = SAM_ID_BLUETOOTH;
		action.actionID = 0;
		action.paramMask = 0;
		for(int i = 0; i < MAX_PARAM; i++){
			action.param[i] = 0;
		}
		action.param[0] = 0x43;
		action.param[1] = 0xFF;
		action.param[2] = 0x10;
		rp_am_addAction(&action);

	}else if(strcmp(args, DB_CS_CMD_SHOW_MAC) == 0){

		//Reload from flash
		hw_mac_reload();

		db_cs_printString("MAC ID List:\r");
		db_cs_printString("============\r");
		for(int i = 0; i < SIZEOF_MAC; i++){
			db_cs_printString("ID: ");
			db_cs_printInt(i);
			if(i < 10) db_cs_printString("  ");
			else db_cs_printString(" ");
			db_cs_printMAC(MAC_Addr[i]);
		}

	}else if (strcmp(args, DB_CS_CMD_PROGRAM_MAC) == 0){

		Action action;
		action.actionSAM = SAM_ID_PROGRAMMING;
		action.actionID = SAM_PROGRAMMING_ACT_ID_WRITE_MAC;
		action.paramMask = 0;
		for(int i = 0; i < MAX_PARAM; i++){
			action.param[i] = 0;
		}
		//Get ID
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_PROGRAM_MAC_HELP);
			return;
		}
		//Set ID
		action.param[0] = atoi(args);

		//Get MAC
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_PROGRAM_MAC_HELP);
			return;
		}

		char *macP = strtok(args, ":");
		int i = 1;
		while(macP != NULL) {
			if(i <= 6){
				action.param[i] = strtol(macP, NULL, 16);
			}else{
				db_cs_printString("Invalid MAC\r");
				return;
			}
			i++;
			macP = strtok(NULL, ":");
		}

		rp_am_addAction(&action);

	}else if (strcmp(args, DB_CS_CMD_SEND_BL) == 0){


		//Get TargetID
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_SEND_BL_HELP);
			return;
		}
		uint8_t targetID = atoi(args);
		if(targetID >= SIZEOF_MAC){
			db_cs_printString("Invalid MAC ID ! \r");
			return;
		}

		//Get CharID
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_SEND_BL_HELP);
			return;
		}
		uint8_t charID = atoi(args);

		//Get Data
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_SEND_BL_HELP);
			return;
		}

		db_cs_printString("Send Bluetooth Packet to ID ");
		db_cs_printInt(targetID);
		db_cs_printString(" :\r -> ");
		db_cs_printMAC(MAC_Addr[targetID]);
		db_cs_printString(" -> Char: ");
		db_cs_printInt(charID);
		db_cs_printString("\r -> Data: ");
		db_cs_printString(args);
		db_cs_printString("\r");

		//Prepare UUID
		uint8_t tx_uuid[16];
		bl_gatt_getUUID(tx_uuid, charID);

		//Prepare Data
		uint8_t data[strlen(args)/2];
		for(int i = 0; i < strlen(args); i += 2){
			char split[3];
			split[0] = args[i];
			split[1] = args[i+1];
			split[2] = '\0';

			data[i/2] = strtol(split, NULL, 16);
		}

		//Send
		hw_bl_sendPacket(MAC_Addr[targetID], strlen(args)/2, data, tx_uuid);

	} else if (strcmp(args, DB_CS_CMD_TRIG_ACTION) == 0) {

		Action action;
		action.paramMask = 0;

		//Get SamID
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_TRIG_ACTION_HELP);
			return;
		}
		action.actionSAM = strtol(args, NULL, 10);

		if(action.actionSAM >= SAM_NUM){
			db_cs_printString("Invalid SAM ID ! \r");
			return;
		}

		//Get ActionID
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_TRIG_ACTION_HELP);
			return;
		}
		action.actionID = strtol(args, NULL, 10);

		//Get Data
		args = strtok(NULL, " ");
		if(args == NULL){
			db_cs_printString(DB_CS_CMD_TRIG_ACTION_HELP);
			return;
		}
		db_cs_printString("Trigger Action:\rSAM ID: ");
		db_cs_printInt(action.actionSAM);
		db_cs_printString("\rAction ID: ");
		db_cs_printInt(action.actionID);
		db_cs_printString("\r Param: ");
		db_cs_printString(args);
		db_cs_printString("\r");

		//Prepare Data
		uint8_t param[strlen(args)/2];
		if(strlen(args)/2 > MAX_PARAM) {
			db_cs_printString("Param to long");
			return;
		}
		for(int i = 0; i < strlen(args); i += 2){
			char split[3];
			split[0] = args[i];
			split[1] = args[i+1];
			split[2] = '\0';

			param[i/2] = strtol(split, NULL, 16);
		}

		for(uint8_t i = 0; i < MAX_PARAM; i++) {
			if(i >= (sizeof(param)/sizeof(uint8_t))) {
				action.param[i] = 0;
			} else {
				action.param[i] = param[i];
			}
		}

		rp_am_addAction(&action);

	}else if(strcmp(args, DB_CS_CMD_SHOW_ACTIONS) == 0){

		db_cs_printString("ACTIONS List:\r");
		db_cs_printString("============\r");
		db_cs_printString("saved at address ");
		db_cs_printInt(_MEMORY_ACTIONS_ADDR);
		db_cs_printString("\r");
		for(int i = 0; i < SIZEOF_PROG_ACTIONS; i++){
			db_cs_printString("ID: ");
			db_cs_printInt(i);
			if(i < 10) db_cs_printString("  ");
			else db_cs_printString(" ");
			db_cs_printAction(&progActions[i]);
		}

	}else if(strcmp(args, DB_CS_CMD_SHOW_RULES) == 0){

		db_cs_printString("RULES List:\r");
		db_cs_printString("============\r");
		db_cs_printString("saved at address ");
		db_cs_printInt(_MEMORY_RULES_ADDR);
		db_cs_printString("\r");
		for(int i = 0; i < SIZEOF_RULES; i++){
			db_cs_printString("ID: ");
			db_cs_printInt(i);
			if(i < 10) db_cs_printString("  ");
			else db_cs_printString(" ");
			db_cs_printRule(&progRules[i]);
		}

	}else if(strcmp(args, DB_CS_CMD_WHATSLIVE) == 0){
		db_cs_printString("42\r");
	}else{
		db_cs_printString("Unknown Command !\r");
	}

}

void db_cs_printString(char *message)
{
	hw_uart_sendString(message);
}

void db_cs_printInt(uint32_t i){
	char c[10];
	db_cs_itoa(i, c);
	hw_uart_sendString(c);
}

void db_cs_printMAC(uint8_t *mac){
	db_cs_printString("MAC: ");
	for(int i = 5; i >= 0; i--){
		uint8_t temp = mac[i];
		char res[3];
		res[0] = TO_HEX(((temp & 0xF0) >> 4));
		res[1] = TO_HEX((temp & 0x0F));
		res[2] = '\0';
		db_cs_printString(res);
		if(i != 0) db_cs_printString(" : ");
	}
	db_cs_printString("\r");
}


void db_cs_printAction(Action *action){
	db_cs_printString("Action: ");
	db_cs_printString("SAM:");
	db_cs_printInt(action->actionSAM);
	db_cs_printString(" ID:");
	db_cs_printInt(action->actionID);
	db_cs_printString(" MASK:");
	db_cs_printInt(action->paramMask);
	db_cs_printString(" PARMS: [");
	for(int i = 0; i < MAX_PARAM; i++){
		db_cs_printString(" ");
		db_cs_printInt(action->param[i]);
	}
	db_cs_printString(" ]\r");
}

void db_cs_printSource(Source *source){
	db_cs_printString("Source: ");
	db_cs_printString("SAM: ");
	db_cs_printInt(source->sourceSAM);
	db_cs_printString(" SOURCEID: ");
	db_cs_printInt(source->sourceID);
	db_cs_printString(" PARAMS: [");
	for(int i = 0; i < MAX_PARAM; i++){
		db_cs_printString(" ");
		db_cs_printInt(source->param[i]);
	}
	db_cs_printString("]\r");
}

void db_cs_printRule(Rule *rule){
	db_cs_printString("Rule: ");
	db_cs_printString("TRIG_SAM: ");
	db_cs_printInt(rule->sourceSAM);
	db_cs_printString(" TRIG_SOURCEID: ");
	db_cs_printInt(rule->sourceID);
	db_cs_printString(" PARAMS: [");
	for(int i = 0; i < MAX_PARAM; i++){
		db_cs_printString(" ");
		db_cs_printInt(rule->param[i]);
	}
	db_cs_printString(" ] PARAMS COMP: [");
	for(int i = 0; i < MAX_PARAM; i++){
		db_cs_printString(" ");
		db_cs_printInt(rule->paramComp[i]);
	}


	db_cs_printString("] ----> TRIG_ACTION: ");
	db_cs_printInt(rule->actionMemID);
	db_cs_printString("\r");
}

char* db_cs_itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
