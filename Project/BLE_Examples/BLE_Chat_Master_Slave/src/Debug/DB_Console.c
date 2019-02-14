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
#include "SourceActionManager/SAM_Bluetooth.h"

char* itoa(int i, char b[]);

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
		action.targetID = 100;
		action.paramNum = 3;
		action.param[0] = 0x43;
		action.param[1] = 0xFF;
		action.param[2] = 0x10;
		sam_bl_triggerAction(action);
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
	itoa(i, c);
	hw_uart_sendString(c);
}

char* itoa(int i, char b[]){
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
