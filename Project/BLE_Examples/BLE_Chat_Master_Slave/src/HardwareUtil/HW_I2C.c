/*
 * I2C.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HardwareUtil/HW_I2C.h"
#include "hal_types.h"
#include "BlueNRG1_i2c.h"

#include "Debug/DB_Console.h"

void hw_i2c_init(){

	db_cs_printString("Init I2C...\r");
	I2C_InitType initStruct = {
			5000, //Operation Frequency (Hz)
			I2C_OperatingMode_Master, //Operation Mode (OM)
			I2C_Filtering_Off, //Digital Filter (FON)
			0xA0, //Own Address
			DISABLE //Addressing Mode (SAM) - 7bit
	};
	I2C_Init(1, initStruct);

	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_I2C1, ENABLE);

	db_cs_printString("Start test transmittion...\r");

//	typedef struct {
//	  uint16_t Operation;               /*!< Specifies the transaction operation: read or write.
//	                                         This parameter can be a value of @ref I2C_operation */
//
//	  uint16_t Address;                 /*!< Slave address. */
//
//	  uint16_t StartByte;               /*!< Enables or disables the start byte.
//	                                         This parameter can be a value of @ref I2C_start_byte */
//
//	  uint16_t AddressType;             /*!< Specifies the address type.
//	                                         This parameter can be a value of @ref I2C_address_type */
//
//	  uint16_t StopCondition;           /*!< Enables or disables the stop condition.
//	                                         This parameter can be a value of @ref I2C_stop_condition */
//
//	  uint16_t Length;                  /*!< Specifies the length of the transaction (number of bytes).
//	                                         This parameter can be a value of @ref I2C_stop_condition */
//	} I2C_TransactionType;

	I2C_BeginTransaction(1, );

}
