/*
 * DB_Console.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_DEBUG_DB_CONSOLE_H_
#define BLUEHOME_DEBUG_DB_CONSOLE_H_

#include <stdint.h>

#define DB_CS_CMD_GETVERSION	"version"
#define DB_CS_CMD_SENDTESTBL	"testBL"

void db_cs_executeCommand(char *cmd);

void db_cs_printString(char *message);
void db_cs_printInt(uint32_t i);
void db_cs_printMAC(uint8_t *mac);

#endif /* BLUEHOME_DEBUG_DB_CONSOLE_H_ */
