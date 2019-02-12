/*
 * DB_Assert.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_DEBUG_DB_ASSERT_H_
#define BLUEHOME_DEBUG_DB_ASSERT_H_

#include <stdint.h>

uint32_t PIN_ERR_FLAG;
uint8_t ADDR_ERR_FLAG;

void db_as_init();
uint8_t db_as_checkInit();

#endif /* BLUEHOME_DEBUG_DB_ASSERT_H_ */
