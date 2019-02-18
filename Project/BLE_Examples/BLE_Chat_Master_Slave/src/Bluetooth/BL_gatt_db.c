/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : gatt_db.c
* Author             : BlueNRG-1 main file for Chat demo
* Version            : V1.0.0
* Date               : 16-September-2015
* Description        : Functions to build GATT DB and handle GATT events.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "Bluetooth/BL_gatt_db.h"
#include "Main/BlueHomeApp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RuleProcess/RP_InterruptManager.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "clock.h"
#include "gp_timer.h"
#include "osal.h"
#include "SDK_EVAL_Config.h"

#include "LPS25HB.h"
#include "lsm6ds3.h"

#include "HardwareUtil/HW_Bluetooth.h"
#include "Debug/DB_Assert.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

#define COPY_SERVICE_CMD_UUID(uuid_struct) 		COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1a)
#define COPY_CHAR_CMD_UUID(uuid_struct)  		COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_CHAR_POLL_UUID(uuid_struct)  		COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1c)

#define COPY_SERVICE_INFO_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x2a)
#define COPY_CHAR_ERROR_UUID(uuid_struct)  		COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x2b)
#define COPY_CHAR_FIRMWARE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x2c)
#define COPY_CHAR_HARDWARE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x2d)
#define COPY_CHAR_HWID_UUID(uuid_struct)  		COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x2e)

#define COPY_SERVICE_DIRECT_UUID(uuid_struct) 	COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x3a)
#define COPY_CHAR_PARAM_UUID(uuid_struct)  		COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x3b)
#define COPY_CHAR_PARAMCOMP_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x3c)
#define COPY_CHAR_OPTIONS_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x3d)

uint16_t infoServHandle, errorCharHandle, fwVerCharHandle, hwVerCharHandle, hwIdCharHandle;
uint16_t cmdServHandle, cmdCharHandle, pollCharHandle;
uint16_t directServHandle, paramCharHandle, paramCompCharHandle, optionsCharHandle;

/* UUIDS */
Service_UUID_t service_uuid;
Char_UUID_t char_uuid;
Char_Desc_Uuid_t char_desc_uuid;

extern uint16_t connection_handle;


tBleStatus bl_gatt_addHWInfoService(void)
{
  tBleStatus ret;
  uint8_t uuid[16];

  COPY_SERVICE_INFO_UUID(uuid);

  //Add Info Service
  Osal_MemCpy(&service_uuid.Service_UUID_128, uuid, 16);
	ret = aci_gatt_add_service(UUID_TYPE_128, &service_uuid, PRIMARY_SERVICE,
			Services_Max_Attribute_Records[INFO_SERVICE_INDEX],
			&infoServHandle);
	if (ret != BLE_STATUS_SUCCESS)
		goto fail;

	// Add Characteristics

  //Error Char
  COPY_CHAR_ERROR_UUID(uuid);
  Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
  ret =  aci_gatt_add_char(infoServHandle, UUID_TYPE_128, &char_uuid, 1, CHAR_PROP_READ, ATTR_PERMISSION_NONE, 0,
                           16, 0, &errorCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  //FW Version Char
  COPY_CHAR_FIRMWARE_UUID(uuid);
  Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
  ret =  aci_gatt_add_char(infoServHandle, UUID_TYPE_128, &char_uuid, 4, CHAR_PROP_READ, ATTR_PERMISSION_NONE, 0,
                           16, 0, &fwVerCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  //HW Version Char
  COPY_CHAR_HARDWARE_UUID(uuid);
  Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
  ret =  aci_gatt_add_char(infoServHandle, UUID_TYPE_128, &char_uuid, 1, CHAR_PROP_READ, ATTR_PERMISSION_NONE, 0,
                           16, 0, &hwVerCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  //HWID Char
  COPY_CHAR_HWID_UUID(uuid);
  Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
  ret =  aci_gatt_add_char(infoServHandle, UUID_TYPE_128, &char_uuid, 1, CHAR_PROP_READ, ATTR_PERMISSION_NONE, 0,
                           16, 0, &hwIdCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  return BLE_STATUS_SUCCESS;

 fail:
  db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error at Info Service");
  return BLE_STATUS_ERROR;

}

tBleStatus bl_gatt_addCMDService(void)
{
	tBleStatus ret;
	uint8_t uuid[16];

	COPY_SERVICE_CMD_UUID(uuid);

	//Add CMD Service
	Osal_MemCpy(&service_uuid.Service_UUID_128, uuid, 16);
	ret = aci_gatt_add_service(UUID_TYPE_128, &service_uuid, PRIMARY_SERVICE,
			Services_Max_Attribute_Records[CMD_SERVICE_INDEX],
			&cmdServHandle);
	if (ret != BLE_STATUS_SUCCESS){
		goto fail;
	}
	// Add Characteristics

	//CMD Char
	COPY_CHAR_CMD_UUID(uuid);
	Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
	ret =  aci_gatt_add_char(cmdServHandle, UUID_TYPE_128, &char_uuid, 20, CHAR_PROP_NOTIFY | CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
						   16, 0, &cmdCharHandle);
	if (ret != BLE_STATUS_SUCCESS){
		goto fail;
	}

	return BLE_STATUS_SUCCESS;

	fail:
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error at CMD Service");
		return BLE_STATUS_ERROR;
}

tBleStatus bl_gatt_addDirectService(void)
{
	tBleStatus ret;
	uint8_t uuid[16];

	COPY_SERVICE_DIRECT_UUID(uuid);

	//Add Direct Service
	Osal_MemCpy(&service_uuid.Service_UUID_128, uuid, 16);
	ret = aci_gatt_add_service(UUID_TYPE_128, &service_uuid, PRIMARY_SERVICE,
			Services_Max_Attribute_Records[DIRECT_SERVICE_INDEX],
			&directServHandle);
	if (ret != BLE_STATUS_SUCCESS){
		goto fail;
	}
	// Add Characteristics

	//PARAM Char
	COPY_CHAR_PARAM_UUID(uuid);
	Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
	ret =  aci_gatt_add_char(directServHandle, UUID_TYPE_128, &char_uuid, 20, CHAR_PROP_NOTIFY | CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
						   16, 0, &paramCharHandle);
	if (ret != BLE_STATUS_SUCCESS){
		goto fail;
	}

	//PARAMCOMP Char
	COPY_CHAR_PARAM_UUID(uuid);
	Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
	ret =  aci_gatt_add_char(directServHandle, UUID_TYPE_128, &char_uuid, 20, CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
						   16, 0, &paramCompCharHandle);
	if (ret != BLE_STATUS_SUCCESS){
		goto fail;
	}

	//OPTIONS Char
	COPY_CHAR_PARAM_UUID(uuid);
	Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
	ret =  aci_gatt_add_char(directServHandle, UUID_TYPE_128, &char_uuid, 20,  CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
						   16, 0, &optionsCharHandle);
	if (ret != BLE_STATUS_SUCCESS){
		goto fail;
	}

	return BLE_STATUS_SUCCESS;

	fail:
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error at Direct Service");
		return BLE_STATUS_ERROR;

}

tBleStatus bl_gatt_initHWInfo()
{
	char buff[4];
	tBleStatus ret;

	buff[0] = 0x00; // NO ERROR
	ret = aci_gatt_update_char_value(infoServHandle, errorCharHandle, 0, 1, (uint8_t *) buff);
	if (ret != BLE_STATUS_SUCCESS){
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while updating Error characteristic");
		return BLE_STATUS_ERROR;
	}

	strcpy(buff, BH_FIRMWARE_VERISON);
	ret = aci_gatt_update_char_value(infoServHandle, fwVerCharHandle, 0, 4, (uint8_t *) buff);
	if (ret != BLE_STATUS_SUCCESS){
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while updating FW Version characteristic");
		return BLE_STATUS_ERROR;
	}

	buff[0] = HW_VERSION;
	ret = aci_gatt_update_char_value(infoServHandle, hwVerCharHandle, 0, 1, (uint8_t *) buff);
	if (ret != BLE_STATUS_SUCCESS){
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while updating HW Version characteristic");
		return BLE_STATUS_ERROR;
	}

	buff[0] = HW_ID;
	ret = aci_gatt_update_char_value(infoServHandle, hwIdCharHandle, 0, 1, (uint8_t *) buff);
	if (ret != BLE_STATUS_SUCCESS){
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while updating HW ID Version characteristic");
		return BLE_STATUS_ERROR;
	}

  return BLE_STATUS_SUCCESS;

}
