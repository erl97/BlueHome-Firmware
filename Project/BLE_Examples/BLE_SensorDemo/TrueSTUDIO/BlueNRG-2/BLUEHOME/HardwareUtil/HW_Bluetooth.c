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
#include "Bluetooth/BL_gatt_db.h"
#include "Bluetooth/BL_config.h"
#include <stdio.h>
#include <string.h>
#include "RuleProcess/RP_InterruptManager.h"
#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"
#include "HW_Bluetooth.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h"
#include "bluenrg1_stack.h"
#include "gp_timer.h"
#include "SDK_EVAL_Config.h"
#ifndef SENSOR_EMULATION
#include "LPS25HB.h"
#include "lsm6ds3.h"
#include "lsm6ds3_hal.h"
#endif
#include "OTA_btl.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define UPDATE_CONN_PARAM 0
#define  ADV_INTERVAL_MIN_MS  1000
#define  ADV_INTERVAL_MAX_MS  1200

#define BLE_SENSOR_VERSION_STRING "1.1.0"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t hw_bl_setConnectable = 1;
uint16_t connection_handle = 0;
uint8_t connInfo[20];

BOOL sensor_board = FALSE; // It is True if sensor boad has been detected

int connected = FALSE;
#if UPDATE_CONN_PARAM
#define UPDATE_TIMER 2 //TBR
int l2cap_request_sent = FALSE;
static uint8_t l2cap_req_timer_expired = FALSE;
#endif

uint8_t Services_Max_Attribute_Records[NUMBER_OF_APPLICATION_SERVICES] = { MAX_NUMBER_ATTRIBUTES_RECORDS_SERVICE_1,	MAX_NUMBER_ATTRIBUTES_RECORDS_SERVICE_2 };

uint8_t sam_bl_initLink();


void hw_bl_init()
{
	db_cs_printString("Init Bluetooth Stack...\r");

	uint8_t ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH,
				"Error in BlueNRG_Stack_Initialization()");
		BLUETOOTH_ERR_FLAG = 1;
	}

	ret = sam_bl_initLink(); //Sensor_DeviceInit();
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
	uint8_t bdaddr[] =
	{ 0x12, 0x34, 0x00, 0xE1, 0x80, 0x02 };
	uint8_t ret;
	uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
	uint8_t device_name[] =
	{ 'B', 'l', 'u', 'e', 'N', 'R', 'G' };

	/* Set the device public address */
	ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
	CONFIG_DATA_PUBADDR_LEN, bdaddr);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH,
				"aci_hal_write_config_data() failed");
		return ret;
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
	ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle,
			&dev_name_char_handle, &appearance_char_handle);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "aci_gap_init() failed");
		char c = (char) ret;
		db_cs_printString(&c);
		return ret;
	}

	/* Update device name */
	ret = aci_gatt_update_char_value_ext(0, service_handle,
			dev_name_char_handle, 0, sizeof(device_name), 0,
			sizeof(device_name), device_name);
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

	/* Add Environmental Sensor Service */
//	ret = Add_Environmental_Sensor_Service();
//	if (ret == BLE_STATUS_SUCCESS)
//	{
//		db_cs_printString("Add_Env_Service() --> SUCCESS\r");
//	}
//	else
//	{
//		db_as_assert(DB_AS_ERROR_BLUETOOTH, "Error in Add_Env_Service");
//		return ret;
//	}

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
	uint8_t local_name[] =
	{ AD_TYPE_COMPLETE_LOCAL_NAME, 'B', 'l', 'u', 'e', 'N', 'R', 'G' };

	hci_le_set_scan_response_data(0, NULL);

	ret = aci_gap_set_discoverable(ADV_IND, (ADV_INTERVAL_MIN_MS * 1000) / 625,
			(ADV_INTERVAL_MAX_MS * 1000) / 625,
			STATIC_RANDOM_ADDR, NO_WHITE_LIST_USE, sizeof(local_name),
			local_name, 0, NULL, 0, 0);
	if (ret != BLE_STATUS_SUCCESS)
	{
		db_as_assert(DB_AS_ERROR_BLUETOOTH, "aci_gap_set_discoverable() failed: 0x%02x");
	}
	else
		db_cs_printString("aci_gap_set_discoverable() --> SUCCESS\r");
}



/* ***************** BlueNRG-1 Stack Callbacks ********************************/

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

	db_cs_printString("Connect !\r");

#if UPDATE_CONN_PARAM
	l2cap_request_sent = FALSE;
	HAL_VTimerStart_ms(UPDATE_TIMER, CLOCK_SECOND*2);
	{
		l2cap_req_timer_expired = FALSE;
	}
#endif

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
	db_cs_printString("Modify\r");
}

//void Attribute_Modified_CB(uint16_t handle, uint16_t data_length, uint8_t *attData){
//	db_cs_printString("ATT Modify\r");
//}


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

