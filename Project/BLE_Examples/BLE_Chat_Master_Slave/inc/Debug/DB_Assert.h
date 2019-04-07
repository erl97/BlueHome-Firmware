/*
 * DB_Assert.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_DEBUG_DB_ASSERT_H_
#define BLUEHOME_DEBUG_DB_ASSERT_H_

#include <stdint.h>

#define DB_AS_ERROR_BOVERFLOW 		1
#define DB_AS_ERROR_BLUETOOTH 		2
#define DB_AS_ERROR_VALUEFCT		3
#define DB_AS_ERROR_ACTIONFCT		4
#define DB_AS_ERROR_PROGRAM			5
#define DB_AS_ERROR_CONFIG			6

char *errorMsg[32];

// INIT ERROR FLAGS //
uint32_t PIN_ERR_FLAG;
uint8_t ADDR_ERR_FLAG;
uint8_t BLUETOOTH_ERR_FLAG;
uint8_t CONFIG_ERR_FLAG;
//////////////////////

void db_as_init();
uint8_t db_as_checkInit();

void db_as_assert(uint8_t ERROR, char *message);

#endif /* BLUEHOME_DEBUG_DB_ASSERT_H_ */
