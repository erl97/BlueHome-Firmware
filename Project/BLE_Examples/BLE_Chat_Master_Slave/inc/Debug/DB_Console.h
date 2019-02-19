/*
 * DB_Console.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_DEBUG_DB_CONSOLE_H_
#define BLUEHOME_DEBUG_DB_CONSOLE_H_

#include <stdint.h>

#include "RuleProcess/RP_Types.h"

#define DB_CS_CMD_GETVERSION	"version"
#define DB_CS_CMD_SENDTESTBL	"testBL"

#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

void db_cs_executeCommand(char *cmd);

void db_cs_printString(char *message);
void db_cs_printInt(uint32_t i);
void db_cs_printMAC(uint8_t *mac);
void db_cs_printAction(Action* action);
void db_cs_printSource(Source *source);

char* db_cs_itoa(int i, char b[]);

#endif /* BLUEHOME_DEBUG_DB_CONSOLE_H_ */
