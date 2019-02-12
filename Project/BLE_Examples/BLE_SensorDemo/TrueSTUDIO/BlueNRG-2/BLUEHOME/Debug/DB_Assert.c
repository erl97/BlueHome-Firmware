/*
 * DB_Assert.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "DB_Assert.h"

void db_as_init()
{
	PIN_ERR_FLAG = 0;
	ADDR_ERR_FLAG = 0;
}

uint8_t db_as_checkInit()
{
	if (PIN_ERR_FLAG != 0 || ADDR_ERR_FLAG != 0)
		return 1;
	else
		return 0;
}