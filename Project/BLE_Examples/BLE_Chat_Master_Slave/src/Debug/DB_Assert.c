/*
 * DB_Assert.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "Debug/DB_Assert.h"

#include "Debug/DB_Console.h"

void db_as_init()
{
	// Reset Init Error Flags
	PIN_ERR_FLAG = 0;
	ADDR_ERR_FLAG = 0;
	BLUETOOTH_ERR_FLAG = 0;
	CONFIG_ERR_FLAG = 0;

	// SET ERROR MSGs //
	errorMsg[DB_AS_ERROR_BOVERFLOW] = "OVERFLOW ERROR";
	errorMsg[DB_AS_ERROR_BLUETOOTH] = "BLUETOOTH ERROR";
	errorMsg[DB_AS_ERROR_VALUEFCT] = "VALUEFCT ERROR";
	errorMsg[DB_AS_ERROR_ACTIONFCT] = "ACTIONFCT ERROR";
	errorMsg[DB_AS_ERROR_PROGRAM] = "PROGRAM ERROR";
	errorMsg[DB_AS_ERROR_CONFIG] = "CONFIG ERROR";
}


uint8_t db_as_checkInit()
{
	if (PIN_ERR_FLAG != 0 || ADDR_ERR_FLAG != 0 || BLUETOOTH_ERR_FLAG != 0 || CONFIG_ERR_FLAG != 0)
		return 1;
	else
		return 0;
}

void db_as_assert(uint8_t ERROR, char *message)
{
	db_cs_printString(errorMsg[ERROR]);
	db_cs_printString(": ");
	db_cs_printString(message);
	db_cs_printString("\r");
	//db_cs_printString(">");
}
