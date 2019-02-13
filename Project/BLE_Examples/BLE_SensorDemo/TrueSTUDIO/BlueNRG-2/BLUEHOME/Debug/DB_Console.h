/*
 * DB_Console.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_DEBUG_DB_CONSOLE_H_
#define BLUEHOME_DEBUG_DB_CONSOLE_H_

#define DB_CS_CMD_GETVERSION	"version"

void db_cs_executeCommand(char *cmd);

void db_cs_printString(char *message);

#endif /* BLUEHOME_DEBUG_DB_CONSOLE_H_ */
