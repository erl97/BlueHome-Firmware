/*
 * DB_Console.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "DB_Console.h"

#include "string.h"

#include "Main/BlueHomeApp.h"
#include "HardwareUtil/HW_UART.h"


void db_cs_executeCommand(char *cmd)
{
	if (strcmp(cmd, DB_CS_CMD_GETVERSION) == 0)
	{
		db_cs_printString("BlueHome Fimware Version ");
		db_cs_printString(BH_FIRMWARE_VERISON);
		db_cs_printString("\r");
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
