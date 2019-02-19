/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
 * File Name          : sensor.c
 * Author             : AMS - VMA RF Application team
 * Version            : V1.0.0
 * Date               : 23-November-2015
 * Description        : Sensor init and sensor state machines
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "HardwareUtil/HW_Bluetooth.h"
#include "HardwareUtil/HW_MAC.h"
#include "HardwareUtil/HW_UART.h"

#include <stdio.h>
#include <string.h>

#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "gp_timer.h"
#include "osal.h"

#include "Bluetooth/BL_gatt_db.h"
#include "Bluetooth/BL_config.h"

#include "SourceActionManager/SAM_Init.h"
#include "SourceActionManager/SAM_Programming.h"

#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"

#include "gp_timer.h"

#define  UPDATE_CONN_PARAM 0
#define  ADV_INTERVAL_MIN_MS  1000
#define  ADV_INTERVAL_MAX_MS  1200

#define BLE_SENSOR_VERSION_STRING "1.1.0"

typedef struct discoveryContext_s {
  uint8_t check_disc_proc_timer;
  uint8_t check_disc_mode_timer;
  uint8_t is_device_found;
  uint8_t do_connect;
  tClockTime startTime;
  uint8_t device_found_address_type;
  uint8_t device_found_address[6];
  uint16_t device_state;
} discoveryContext_t;

volatile uint8_t hw_bl_setConnectable = 1;
volatile uint16_t connection_handle = 0;

//Handels
extern uint16_t infoServHandle, errorCharHandle, fwVerCharHandle, hwVerCharHandle, hwIdCharHandle;
extern uint16_t cmdServHandle, cmdCharHandle;
extern uint16_t directServHandle, paramCharHandle, paramCompCharHandle, optionsCharHandle;

uint16_t service_handle, dev_name_char_handle, appearance_char_handle;


volatile int connected = FALSE;
volatile uint8_t transmitionDone = 0;
volatile uint16_t txHandle = 0;
volatile uint8_t receivedChar = 0;

uint8_t Services_Max_Attribute_Records[NUMBER_OF_APPLICATION_SERVICES] = { MAX_NUMBER_ATTRIBUTES_RECORDS_SERVICE_1,	MAX_NUMBER_ATTRIBUTES_RECORDS_SERVICE_2, MAX_NUMBER_ATTRIBUTES_RECORDS_SERVICE_3};

uint8_t initMacCandidate[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

extern uint8_t defaultAddr[6];
extern uint8_t DEVICE_BDADDR[6];
extern uint8_t DEVICE_NAME[16];

uint8_t directParam[MAX_PARAM];
uint8_t directCompParam[MAX_PARAM];
uint32_t directMask;
uint8_t directActionMemID;
uint8_t directRuleMemID;
uint8_t directActionSAMID;
uint8_t directActionID;
uint8_t directSourceSAMID;
uint8_t directSourceID;

uint8_t sam_bl_initLink();

void hw_bl_init()
{
	db_cs_printString("Init Bluetooth Stack...\r");
	db_cs_printMAC(DEVICE_BDADDR);

	tBleStatus ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH,
				"Error in BlueNRG_Stack_Initialization()");
		BLUETOOTH_ERR_FLAG = 1;
	}

	ret = sam_bl_initLink();
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Bluetooth Init failed");
		BLUETOOTH_ERR_FLAG = 1;
	}
	else
	{
		db_cs_printString("Bluetooth Init successfully !\r");
	}

}

uint8_t sam_bl_initLink()
{

	tBleStatus ret;

	/* Set the device public address */
	ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
	CONFIG_DATA_PUBADDR_LEN, DEVICE_BDADDR);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH,
				"aci_hal_write_config_data() failed");
	}

	/* Set the TX power -2 dBm */
	aci_hal_set_tx_power_level(1, 4);

	/* GATT Init */
	ret = aci_gatt_init();
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "aci_gatt_init() failed");
		return ret;
	}

	/* GAP Init */
	ret = aci_gap_init(GAP_CENTRAL_ROLE|GAP_PERIPHERAL_ROLE, 0, 0x10, &service_handle,
			&dev_name_char_handle, &appearance_char_handle);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "aci_gap_init() failed");
		db_cs_printInt(ret);
		return ret;
	}

	/* Update device name */
	ret = aci_gatt_update_char_value_ext(0, service_handle,
			dev_name_char_handle, 0, sizeof(DEVICE_NAME), 0,
			sizeof(DEVICE_NAME), DEVICE_NAME);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH,
				"aci_gatt_update_char_value_ext() failed");
		return ret;
	}

	ret = aci_gap_set_authentication_requirement(BONDING,
	MITM_PROTECTION_REQUIRED,
	SC_IS_NOT_SUPPORTED,
	KEYPRESS_IS_NOT_SUPPORTED, 7, 16,
	USE_FIXED_PIN_FOR_PAIRING, 123456, 0x00);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH,
				"aci_gap_set_authentication_requirement() failed");
		return ret;
	}

	db_cs_printString("BLE Stack Initialized with SUCCESS\r");


	// ADD SERVICES //

	/* Add HW/Firmware INFO service and Characteristics */
	ret = bl_gatt_addHWInfoService();
	if (ret == BLE_STATUS_SUCCESS)
	{
		db_cs_printString("Add BL Info Service --> SUCCESS\r");
	}
	else
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error adding BL Info Service");
		return ret;
	}

	/* Add HW/Firmware CMD service and Characteristics */
	ret = bl_gatt_addCMDService();
	if (ret == BLE_STATUS_SUCCESS)
	{
		db_cs_printString("Add BL CMD Service --> SUCCESS\r");
	}
	else
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error adding BL CMD Service");
		return ret;
	}

	ret = bl_gatt_addDirectService();
	if (ret == BLE_STATUS_SUCCESS)
	{
		db_cs_printString("Add BL Direct Service --> SUCCESS\r");
	}
	else
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error adding BL Direct Service");
		return ret;
	}

	//Search Mac if unitialized
	if(MAC_UNINITIALIZED){
		hw_mac_generateDeviceAddress(initMacCandidate);
		tBleStatus ret = aci_gap_start_general_discovery_proc(0x4000, 0x4000, PUBLIC_ADDR, 0x00);
		if (ret != BLE_STATUS_SUCCESS)
		{
			db_as_assert(DB_AS_ERROR_BLUETOOTH, "aci_gap_start_general_discovery_proc() failed");
		}
		else
		{
			db_cs_printString("aci_gap_start_general_discovery_proc() --> SUCCESS\r");
		}
	}
	/* Start the Sensor Timer */
//	ret = HAL_VTimerStart_ms(SENSOR_TIMER, acceleration_update_rate);
//	if (ret != BLE_STATUS_SUCCESS)
//	{
//		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error in HAL_VTimerStart_ms");
//		return ret;
//	}
//	else
//	{
//		sensorTimer_expired = FALSE;
//	}

	return BLE_STATUS_SUCCESS;

}


/*******************************************************************************
 * Function Name  : Set_DeviceConnectable.
 * Description    : Puts the device in connectable mode.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void hw_bl_setDeviceConnectable(void)
{
	uint8_t ret;
	uint8_t local_name[sizeof(DEVICE_NAME)/sizeof(uint8_t)+1];

	local_name[0] = AD_TYPE_COMPLETE_LOCAL_NAME;
	for(int i = 1; i <= sizeof(DEVICE_NAME)/sizeof(uint8_t); i++){
		local_name[i] = DEVICE_NAME[i-1];
	}

	hci_le_set_scan_response_data(0, NULL);

	ret = aci_gap_set_discoverable(ADV_IND, (ADV_INTERVAL_MIN_MS * 1000) / 625,
			(ADV_INTERVAL_MAX_MS * 1000) / 625,
			PUBLIC_ADDR, NO_WHITE_LIST_USE, sizeof(local_name),
			local_name, 0, NULL, 0, 0);

	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "aci_gap_set_discoverable() failed");
	}
	else
		db_cs_printString("aci_gap_set_discoverable() --> SUCCESS\r");
}

tBleStatus hw_bl_setDeviceNonDiscover(){
	tBleStatus ret = aci_gap_set_non_discoverable();
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "aci_gap_set_non_discoverable() failed");
		return ret;
	}
	else
		db_cs_printString("aci_gap_set_non_discoverable() --> SUCCESS\r");

	return BLE_STATUS_SUCCESS;
}

tBleStatus hw_bl_makeConnection(uint8_t* addr){
	tBleStatus ret = aci_gap_create_connection(0x4000, 0x4000, PUBLIC_ADDR, addr, PUBLIC_ADDR, 40, 40, 0, 60, 2000, 2000);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while starting connection!");
		return ret;
	}else{
		db_cs_printString("Trying to connect...\r");
	}

	uint32_t timeout = 100000;
	while(!connected && timeout != 0){
		BTLE_StackTick();
		timeout--;
	};

	if(timeout == 0){
//		ret = aci_gap_terminate_gap_proc(GAP_GENERAL_CONNECTION_ESTABLISHMENT_PROC);
//		if (ret != BLE_STATUS_SUCCESS)
//		{
//			db_as_assert(DB_AS_ERROR_BLUETOOTH, "Failed to stop connection attempt!");
//			return ret;
//		}else{
//			db_cs_printString("Could not connect !");
//		}

		return BLE_STATUS_TIMEOUT;
	}else return BLE_STATUS_SUCCESS;
}

tBleStatus hw_bl_terminateConnection(){
	if(connected){
		tBleStatus ret = aci_gap_terminate(connection_handle, 0x13);
		if (ret != BLE_STATUS_SUCCESS)
		{
			db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while terminating connection!");
			return ret;
		}

		while(connected){
			BTLE_StackTick();
		};
		return BLE_STATUS_SUCCESS;
	}
	return BLE_STATUS_ERROR;
}

tBleStatus hw_bl_sendPacket(uint8_t* addr, uint8_t data_length, uint8_t* data, uint16_t charHandle){
	db_cs_printString("Sending Packet...\r");

	//Wait for disconnect
	if(connected){
		db_cs_printString("Waiting for disconnect...\r");

		uint32_t timeout = 100000;
		while(connected && timeout != 0){
			BTLE_StackTick();
			timeout--;
		};

		if(timeout == 0){
			hw_bl_terminateConnection();
		}

	}else{

		//Turn off discover
		hw_bl_setDeviceNonDiscover();

	}


	//Make connection
	if(hw_bl_makeConnection(addr) == BLE_STATUS_SUCCESS){

		UUID_t tx_uuid;
		//Get Handle
		const uint8_t charUuid128_TX[16] = {0x1b,0xc5,0xd5,0xa5, 0x02,0x00, 0xb4,0x9a, 0xe1,0x11, 0x3a,0xcf,0x80,0x6e,0x36,0x02}; //UUID from CMD Service
		//COPY_CHAR_CMD_UUID(tx_uuid);

		Osal_MemCpy(&tx_uuid.UUID_128, charUuid128_TX, 16);
		db_cs_printString("GET UUID\r");

		aci_gatt_disc_char_by_uuid(connection_handle, 0x0001, 0xFFFF, UUID_TYPE_128, &tx_uuid);

		while(txHandle == 0){
			BTLE_StackTick();
		}
		txHandle = 0;

		tBleStatus ret = aci_gatt_write_without_resp(connection_handle, txHandle+1, data_length, data);
		//tBleStatus ret = aci_gatt_write_char_value(connection_handle, txHandle+1, MAX_PARAM, d);
		if (ret != BLE_STATUS_SUCCESS)
		{
			db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while writing characteristic !");
			db_cs_printInt(ret);
			db_cs_printString("\r");
		}

		while(transmitionDone == 0){
			BTLE_StackTick();
		};
		transmitionDone = 0;

		//Disconnect
		hw_bl_terminateConnection();

	}else{
		hw_bl_setDeviceConnectable();
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error could not connect to: \r");
		db_cs_printMAC(addr);
	}

//	MAYBE TIMEOUT FOR WRITE IN FLASH
//	for (uint32_t j = 0; j < 10000000; j++)
//	{
//	__NOP();
//	}

	db_cs_printString("Data send successfully\r");
	return 0;
}


/* ***************** BlueNRG-1 Stack Callbacks ********************************/

void aci_att_prepare_write_resp_event(uint16_t Connection_Handle,
                                      uint16_t Attribute_Handle,
                                      uint16_t Offset,
                                      uint8_t Part_Attribute_Value_Length,
                                      uint8_t Part_Attribute_Value[]){
	db_cs_printString("Prepare write\r");
}

void aci_att_exec_write_resp_event(uint16_t Connection_Handle){
	db_cs_printString("Exec write\r");
}

/*******************************************************************************
 * Function Name  : hci_le_connection_complete_event.
 * Description    : This event indicates that a new connection has been created.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_connection_complete_event(uint8_t Status,
		uint16_t Connection_Handle, uint8_t Role, uint8_t Peer_Address_Type,
		uint8_t Peer_Address[6], uint16_t Conn_Interval, uint16_t Conn_Latency,
		uint16_t Supervision_Timeout, uint8_t Master_Clock_Accuracy)
{

	connected = TRUE;
	connection_handle = Connection_Handle;

	db_cs_printString("Connect ! From ");
	db_cs_printMAC(Peer_Address);

	for(int i = 0; i < 6; i++){
		hw_bl_connectedDeviceAddr[i] = Peer_Address[i];
	}

}/* end hci_le_connection_complete_event() */


/*******************************************************************************
 * Function Name  : hci_disconnection_complete_event.
 * Description    : This event occurs when a connection is terminated.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_disconnection_complete_event(uint8_t Status,
		uint16_t Connection_Handle, uint8_t Reason)
{
	connected = FALSE;
	/* Make the device connectable again. */
	hw_bl_setConnectable = TRUE;
	connection_handle = 0;

	db_cs_printString("Disconnect !\r");
}/* end hci_disconnection_complete_event() */

/*******************************************************************************
 * Function Name  : aci_gatt_read_permit_req_event.
 * Description    : This event is given when a read request is received
 *                  by the server from the client.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_read_permit_req_event(uint16_t Connection_Handle,
		uint16_t Attribute_Handle, uint16_t Offset)
{

	db_cs_printString("READ CB\r");
	aci_gatt_allow_read(Connection_Handle);
}

void aci_gatt_write_permit_req_event(uint16_t Connection_Handle,
                                     uint16_t Attribute_Handle,
                                     uint8_t Data_Length,
                                     uint8_t Data[]){
	db_cs_printString("WRITE CB\r");
	aci_gatt_write_resp(Connection_Handle, Attribute_Handle, 0x00, 0x00, Data_Length, Data);

}

/*******************************************************************************
 * Function Name  : aci_gatt_attribute_modified_event.
 * Description    : This event occurs when an attribute is modified.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
		uint16_t Attr_Handle, uint16_t Offset, uint16_t Attr_Data_Length,
		uint8_t Attr_Data[])
{
	db_cs_printString("Receive Packet !\r");

	if(Attr_Handle == cmdCharHandle+1){

		if(Attr_Data[0] > MAX_PARAM){
			rp_sm_triggerSource(SAM_ID_BLUETOOTH, MAX_PARAM, Attr_Data);
			db_as_assert(DB_AS_ERROR_BOVERFLOW, "Param Num to large !");
		}else rp_sm_triggerSource(SAM_ID_BLUETOOTH, Attr_Data[0], Attr_Data);

	}else if(Attr_Handle == optionsCharHandle+1){

		//Build Action
		Action action;

		action.actionSAM = Attr_Data[0];
		action.actionID = Attr_Data[1];

		if(action.actionSAM != SAM_ID_PROGRAMMING){ //Execute Action
			action.paramMask = (Attr_Data[5] << 24) | (Attr_Data[4] << 16) | (Attr_Data[3] << 8) | (Attr_Data[2] << 0);
			action.paramNum = Attr_Data[6];
		}else{ //Programm

			action.paramMask = 0x00;

			if(action.actionID == SAM_PROGRAMMING_ACT_ID_WRITE_RULE){
				directActionMemID = Attr_Data[2];
				directRuleMemID = Attr_Data[3];
				directSourceSAMID = Attr_Data[4];
				directSourceID = Attr_Data[5];
				action.paramNum = Attr_Data[6];

			}else if(action.actionID == SAM_PROGRAMMING_ACT_ID_WRITE_ACTION){
				directMask = (Attr_Data[8] << 24) | (Attr_Data[7] << 16) | (Attr_Data[6] << 8) | (Attr_Data[5] << 0);
				directActionMemID = Attr_Data[2];
				directActionSAMID = Attr_Data[3];
				directActionID = Attr_Data[4];
				action.paramNum = Attr_Data[9];

			}else if(action.actionID == SAM_PROGRAMMING_ACT_ID_WRITE_MAC){
				action.paramNum = Attr_Data[2];
			}
		}

		if(action.paramNum > MAX_PARAM){
			action.paramNum = MAX_PARAM;
			db_as_assert(DB_AS_ERROR_BOVERFLOW, "Param Num to large !");
		}

		for(int i = 0; i < action.paramNum; i++){
			action.param[i] = directParam[i];
		}

		//Add to Action Buffer
		rp_am_addAction(action);

	}else if(Attr_Handle == paramCharHandle+1){
		for(int i = 0; i < Attr_Data_Length; i++){
			directParam[i] = Attr_Data[i];
		}
	}else if(Attr_Handle == paramCompCharHandle+1){
		for(int i = 0; i < Attr_Data_Length; i++){
			directCompParam[i] = Attr_Data[i];
		}
	}

}

void aci_gatt_proc_complete_event(uint16_t Connection_Handle, uint8_t Error_Code){
	if(Error_Code != SUCCESS){
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error while transimitting data");
		db_cs_printString("Error Code: ");
		db_cs_printInt(Error_Code);
		db_cs_printString("\r");
	}
	db_cs_printString("Transmition done\r");
	transmitionDone = 1;
}

void aci_gatt_disc_read_char_by_uuid_resp_event(uint16_t Connection_Handle,
                                                uint16_t Attribute_Handle,
                                                uint8_t Attribute_Value_Length,
                                                uint8_t Attribute_Value[])
{
	db_cs_printString("aci_gatt_disc_read_char_by_uuid_resp_event, Connection Handle\r");
	db_cs_printString("Attributes: ");
	db_cs_printInt(Attribute_Value_Length);
	for(int i = 0; i < Attribute_Value_Length; i++){
		db_cs_printString(" ");
		db_cs_printInt(Attribute_Value[i]);
	}
	db_cs_printString("\r");

 	txHandle = Attribute_Handle;

} /* end aci_gatt_disc_read_char_by_uuid_resp_event() */


void aci_hal_end_of_radio_activity_event(uint8_t Last_State, uint8_t Next_State,
		uint32_t Next_State_SysTime)
{
	db_cs_printString("End of Radio\r");
}


/*******************************************************************************
 * Function Name  : HAL_VTimerTimeoutCallback.
 * Description    : This function will be called on the expiry of
 *                  a one-shot virtual timer.
 * Input          : See file bluenrg1_stack.h
 * Output         : See file bluenrg1_stack.h
 * Return         : See file bluenrg1_stack.h
 *******************************************************************************/
//void HAL_VTimerTimeoutCallback(uint8_t timerNum)
//{
//	if (timerNum == SENSOR_TIMER)
//	{
//		sensorTimer_expired = TRUE;
//	}
//#if UPDATE_CONN_PARAM
//	else if (timerNum == UPDATE_TIMER)
//	{
//		l2cap_req_timer_expired = TRUE;
//	}
//#endif
//}

//tBleStatus ret = aci_gap_start_general_discovery_proc(0x4000, 0x4000, PUBLIC_ADDR, 0x00);

void hci_le_advertising_report_event(uint8_t Num_Reports,
                                     Advertising_Report_t Advertising_Report[])
{

	for(uint8_t i = 0; i < Num_Reports; i++){
		uint8_t evt_type = Advertising_Report[i].Event_Type;
		//uint8_t data_length = Advertising_Report[i].Length_Data;
		//uint8_t bdaddr_type = Advertising_Report[i].Address_Type;
		uint8_t bdaddr[6];

		if (evt_type == ADV_IND){

			Osal_MemCpy(bdaddr, Advertising_Report[0].Address,6);
			db_cs_printString("Found ");
			db_cs_printMAC(bdaddr);

			if(hw_mac_compareMac(bdaddr, initMacCandidate)){
				db_cs_printString("MAC Collision -> selecting new Mac\r");
				hw_mac_generateDeviceAddress(initMacCandidate);
			}

		}
	}
} /* hci_le_advertising_report_event() */

void aci_gap_proc_complete_event(uint8_t Procedure_Code,
                                 uint8_t Status,
                                 uint8_t Data_Length,
                                 uint8_t Data[])
{
	if (Procedure_Code == GAP_GENERAL_DISCOVERY_PROC) {
		db_cs_printString("Scan Complete\r");
		uint8_t bdaddr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

		if(!hw_mac_compareMac(bdaddr, initMacCandidate)){
			db_cs_printString("Setting new MAC\r");
			db_cs_printMAC(initMacCandidate);

			for(int i = 0; i < 6; i++){
				DEVICE_BDADDR[i] = initMacCandidate[i];
			}
			hw_mac_generateDeviceName();

			/* Set the device public address */
			tBleStatus ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
			CONFIG_DATA_PUBADDR_LEN, DEVICE_BDADDR);
			if (ret != BLE_STATUS_SUCCESS)
			{
				db_as_assert(DB_AS_ERROR_BLUETOOTH,
						"aci_hal_write_config_data() failed");
			}

			/* Update device name */
			ret = aci_gatt_update_char_value_ext(0, service_handle,
					dev_name_char_handle, 0, sizeof(DEVICE_NAME), 0,
					sizeof(DEVICE_NAME), DEVICE_NAME);
			if (ret != BLE_STATUS_SUCCESS)
			{
				db_as_assert(DB_AS_ERROR_BLUETOOTH,
						"aci_gatt_update_char_value_ext() failed");
			}

			//Save current MAC
			hw_mac_writeCurrentMactoFlash();
			MAC_UNINITIALIZED = 0;
		}
	}
}

