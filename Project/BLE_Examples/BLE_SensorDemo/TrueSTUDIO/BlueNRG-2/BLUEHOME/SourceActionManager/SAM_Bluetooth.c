/*
 * SAM_Bluetooth.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "SAM_Bluetooth.h"

#include <stdlib.h>

#include "bluenrg1_stack.h"
#include "ble_status.h"

#include "RuleProcess/RP_RuleChecker.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"

#include "RuleProcess/RP_Types.h"


void sam_bl_addServices();
Source sam_bl_generateSource();

void sam_bl_init()
{
	sam_bl_addServices();
}

void sam_bl_notifyEvent()
{
	db_cs_printString("Bluetooth Paket Received !\r");

	rp_rc_addSource(sam_bl_generateSource());
}

Source sam_bl_generateSource()
{
	db_cs_printString("Building Source...\r");

	Source blueSource;

	blueSource.sourceType = SOURCETYPE_BLUETOOTH;
	//blueSource.sourceID = //MAC

	return blueSource;
}

void sam_bl_addServices()
{


	/* Add ACC service and Characteristics */
//	ret = Add_Acc_Service();
//	if (ret == BLE_STATUS_SUCCESS)
//	{
//		db_cs_printString("Acceleration service added successfully\r");
//	}
//	else
//	{
//		db_as_assert(DB_AS_ERROR_BLUETOOTH,
//				"Error while adding Acceleration service");
//		return ret;
//	}
//
//	/* Add Environmental Sensor Service */
//	ret = Add_Environmental_Sensor_Service();
//	if (ret == BLE_STATUS_SUCCESS)
//	{
//		db_cs_printString("Environmental service added successfully\r");
//	}
//	else
//	{
//		db_as_assert(DB_AS_ERROR_BLUETOOTH,
//				"Error while adding Environmental service");
//		return ret;
//	}

}
