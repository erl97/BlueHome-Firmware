/*
 * DB_Console.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "Debug/DB_Console.h"

#include "string.h"
#include <stdint.h>

#include "Main/BlueHomeApp.h"
#include "HardwareUtil/HW_UART.h"

#include "RuleProcess/RP_Types.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "SourceActionManager/SAM_Bluetooth.h"
#include "SourceActionManager/SAM_Init.h"

void db_cs_executeCommand(char *cmd)
{
	if (strcmp(cmd, DB_CS_CMD_GETVERSION) == 0)
	{
		db_cs_printString("BlueHome Fimware Version ");
		db_cs_printString(BH_FIRMWARE_VERISON);
		db_cs_printString("\r");
	}
	else if (strcmp(cmd, DB_CS_CMD_SENDTESTBL) == 0)
	{
		Action action;
		action.actionSAM = SAM_ID_BLUETOOTH;
		action.actionID = 100;
		action.paramNum = 3;
		action.param[0] = 0x43;
		action.param[1] = 0xFF;
		action.param[2] = 0x10;
		rp_am_addAction(action);
	}
	else
	{
		db_cs_printString("Unknown Command !");
		db_cs_printString("\r");
	}
	//db_cs_printString(">");
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
	for(int i = 0; i < action->paramNum; i++){
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
	for(int i = 0; i < source->paramNum; i++){
		db_cs_printString(" ");
		db_cs_printInt(source->param[i]);
	}
	db_cs_printString("]\r");
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
