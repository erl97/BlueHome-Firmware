/*
 * SAM_Bluetooth.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "SourceActionManager/SAM_Bluetooth.h"
#include "SourceActionManager/SAM_Init.h"

#include <stdlib.h>
#include <stdint.h>

#include "bluenrg1_stack.h"
#include "ble_status.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"

#include "RuleProcess/RP_RuleChecker.h"
#include "RuleProcess/RP_Init.h"
#include "RuleProcess/RP_Types.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "HardwareUtil/HW_MAC.h"
#include "HardwareUtil/HW_Bluetooth.h"

#include "Bluetooth/BL_gatt_db.h"


extern uint8_t hw_bl_connectedDeviceAddr[6];
extern uint16_t cmdServHandle, cmdCharHandle, pollCharHandle;

extern uint8_t defaultAddr[6];

void sam_bl_addServices();
Source sam_bl_generateSource(uint8_t attr_data_length, uint8_t *attr_data);

void sam_bl_init()
{

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_BLUETOOTH, sam_bl_triggerSource);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_BLUETOOTH, sam_bl_triggerAction);

	// Register ValueFct

}

void sam_bl_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("Bluetooth Source\r");

	Source blueSource;

	blueSource.sourceSAM = SAM_ID_BLUETOOTH;
	blueSource.sourceID = hw_mac_getMacId(hw_bl_connectedDeviceAddr);

	for(int i = 0; i < MAX_PARAM; i++){
		blueSource.param[i] = param[i];
	}

	rp_rc_addSource(blueSource);
}

void sam_bl_triggerAction(Action *action){

	db_cs_printString("Bluetooth Action\r");

	if(action->actionID == 0){ //Publish on own GATT
		bl_gatt_updateDirectParam(action->param);
	}else{ //Send to other node
		uint8_t bdaddr[6];
		hw_mac_getMac(action->actionID, bdaddr);

		hw_bl_sendPacket(bdaddr, MAX_PARAM, action->param, cmdCharHandle);
	}
}

