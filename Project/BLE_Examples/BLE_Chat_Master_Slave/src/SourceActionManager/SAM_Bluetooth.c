/*
 * SAM_Bluetooth.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "SourceActionManager/SAM_Bluetooth.h"

#include <stdlib.h>
#include <stdint.h>

#include "bluenrg1_stack.h"
#include "ble_status.h"

#include "RuleProcess/RP_RuleChecker.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"

#include "RuleProcess/RP_Types.h"
#include "HardwareUtil/HW_MAC.h"
#include "HardwareUtil/HW_Bluetooth.h"

extern uint8_t hw_bl_connectedDeviceAddr[6];
extern uint16_t cmdServHandle, cmdCharHandle, pollCharHandle;

void sam_bl_addServices();
Source sam_bl_generateSource(uint8_t attr_data_length, uint8_t *attr_data);

void sam_bl_init()
{

}

void sam_bl_notifyEvent(uint8_t attr_data_length, uint8_t *attr_data)
{
	db_cs_printString("Bluetooth Paket Received !\r");

	rp_rc_addSource(sam_bl_generateSource(attr_data_length, attr_data));
}

Source sam_bl_generateSource(uint8_t attr_data_length, uint8_t *attr_data)
{
	db_cs_printString("Building Source...\r");

	Source blueSource;

	blueSource.sourceType = SOURCETYPE_BLUETOOTH;
	blueSource.sourceID = hw_mac_getMacId(hw_bl_connectedDeviceAddr);
	blueSource.paramNum = attr_data_length;

	for(int i = 0; i < blueSource.paramNum; i++){
		blueSource.param[i] = attr_data[i];
	}

	return blueSource;
}

void sam_bl_triggerAction(Action action){

	db_cs_printString("Trigger BL Action...\r");

	//uint8_t addr[6];
	//hw_mac_getMac(action.targetID, addr);
	uint8_t bdaddr[6] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x10}; // Test Connect Addr
	hw_bl_sendPacket(bdaddr, action.paramNum, action.param, cmdCharHandle);

}