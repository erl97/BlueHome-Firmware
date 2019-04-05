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
#define DB_CS_CMD_GETHELP		"help"
#define DB_CS_CMD_SENDTESTBL	"testBL"
#define DB_CS_CMD_SHOW_MAC		"showMacs"
#define DB_CS_CMD_PROGRAM_MAC	"progMac"
#define DB_CS_CMD_SEND_BL		"sendBL"
#define DB_CS_CMD_TRIG_ACTION	"trigAct"

#define DB_CS_CMD_HELP "\
BlueHome Debug Console Help\r\
===========================\r\
Commands:\r\
 showMacs\r\
 progMac <ID> <MAC>\r\
 sendBL <TargetID> <CharID> <Data>\r\
 trigAct <SamID> <ActionID> <Param>\r"

#define DB_CS_CMD_PROGRAM_MAC_HELP	"Usage: progMac <ID> <MAC>\r"
#define DB_CS_CMD_SEND_BL_HELP "\
Usage: sendBL <TargetID> <CharID> <Data>\r\
         CharIDs: 0 - CMD Char\r"
#define DB_CS_CMD_TRIG_ACTION_HELP "Usage: trigAct <SamID> <ActionID> <Param>\r"

#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

void db_cs_executeCommand(char *cmd);

void db_cs_printString(char *message);
void db_cs_printInt(uint32_t i);
void db_cs_printMAC(uint8_t *mac);
void db_cs_printAction(Action* action);
void db_cs_printSource(Source *source);

char* db_cs_itoa(int i, char b[]);

#endif /* BLUEHOME_DEBUG_DB_CONSOLE_H_ */
