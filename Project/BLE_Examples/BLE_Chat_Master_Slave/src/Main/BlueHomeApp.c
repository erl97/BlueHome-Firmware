/*
 * BlueHomeApp.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "Bluetooth/BL_gatt_db.h"
#include "Main/BlueHomeApp.h"

#include "BlueNRG_x_device.h"
#include "bluenrg1_stack.h"


#include "BlueNRG1_mft.h"
/// DEBUG ///
#include "Debug/DB_TestCases.h"
#include "SDK_EVAL_Led.h"
/////////////

#include "HardwareUtil/HW_Init.h"
#include "HardwareUtil/HW_GPIO.h"
#include "HardwareUtil/HW_Bluetooth.h"
#include "HardwareUtil/HW_MAC.h"

#include "SourceActionManager/SAM_Init.h"


#include "RuleProcess/RP_RuleChecker.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "Debug/DB_Console.h"
#include <stdint.h>

//#include "Demo/sensor.h"
#include "bluenrg1_stack.h"
#include "ble_const.h"
#include "sleep.h"
#include "clock.h"

int main(void)
{

	SystemInit();

	// DEBUG CODE //
	db_tc_init_Eval();
	//db_tc_init_GPIO_Int();
	///////////////

	Clock_Init();

	db_as_init();

	hw_init_init();
	hw_gpio_init();

	/* Sensor Device Init */
//	uint8_t ret = Sensor_DeviceInit();
//	if (ret != BLE_STATUS_SUCCESS)
//	{
//		SdkEvalLedOn(LED3);
//		while (1)
//			;
//	}
	//RP INIT//
	rp_init_init();

//	//SAM INITS//
	sam_init_init();

//	//HW INIT//
	hw_init_pins();

	sam_init_initHW();

	bl_gatt_initHWInfo();

	if (db_as_checkInit() == 0)
		db_cs_printString("Initialization successfully ! \r");
	else{
		db_cs_printString("Initialization failed ! \r");
		db_cs_printString("Halting CPU... \r");
		while (1){}
	}
		//SdkEvalLedOn(LED3);


	//uint8_t mac[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
	//hw_mac_writeMacToFlash(mac, 2);

	while (1)
	{
		/* BLE Stack Tick */
		BTLE_StackTick();

		/* Application Tick */
		APP_Tick();

		/* Power Save management */
		//BlueNRG_Sleep(SLEEPMODE_NOTIMER, 0, 0);

	}/* while (1) */



//	if (db_as_checkInit() == 0)
//		SdkEvalLedOn(LED3);
//	else
//	{
//		while (1)
//			;
//		db_cs_printString("Initialization failed ... halting ! \r");
//	}
//
//	db_cs_printString("Initialization completed successfully ! \r");
//
//	while (1)
//	{
//		BTLE_StackTick();
//
//		//APP_Tick();
//
//		//BlueNRG_Sleep(SLEEPMODE_NOTIMER, 0, 0);
//	}
}

/****************** BlueNRG-1 Sleep Management Callback ********************************/

SleepModes App_SleepMode_Check(SleepModes sleepMode)
{
	if (SdkEvalComIOTxFifoNotEmpty())
		return SLEEPMODE_RUNNING;

	return SLEEPMODE_NOTIMER;
}

/***************************************************************************************/

/*******************************************************************************
 * Function Name  : APP_Tick.
 * Description    : Sensor Demo state machine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void APP_Tick(void)
{
	/* Make the device discoverable */
	if (hw_bl_setConnectable && !MAC_UNINITIALIZED)
	{
		hw_bl_setDeviceConnectable();
		hw_bl_setConnectable = FALSE;
	}

	rp_sm_tick();

	rp_rc_tick();

	rp_am_tick();

}

//void APP_Tick(void)
//{

//	//BlueHome
//	if (sourceBuffer[read_idx_source].sourceType != SOURCETYPE_NOSOURCE)
//	{
//		source_handler(sourceBuffer[read_idx_source]);
//		sourceBuffer[read_idx_source].sourceType = SOURCETYPE_NOSOURCE;
//		read_idx_source++;
//		if (read_idx_source == SIZEOFSOURCEBUFFER)
//			read_idx_source = 0;
//	}
//
//
//
//	//ab hier SensorDemo:
//  /* Make the device discoverable */
//  if(set_connectable) {
//    Set_DeviceConnectable();
//    set_connectable = FALSE;
//  }
//
//#if UPDATE_CONN_PARAM
//  /* Connection parameter update request */
//  if(connected && !l2cap_request_sent && l2cap_req_timer_expired){
//    aci_l2cap_connection_parameter_update_req(connection_handle, 9, 20, 0, 600); //24, 24
//    l2cap_request_sent = TRUE;
//  }
//#endif
//
//  /*  Update sensor value */
//  if (sensorTimer_expired) {
//    sensorTimer_expired = FALSE;
//    if (HAL_VTimerStart_ms(SENSOR_TIMER, acceleration_update_rate) != BLE_STATUS_SUCCESS)
//      sensorTimer_expired = TRUE;
//    if(connected) {
//      AxesRaw_t acc_data;
//
//      /* Activity Led */
//      SdkEvalLedOn(LED1);
//
//      /* Get Acceleration data */
//      if (GetAccAxesRaw(&acc_data) == IMU_6AXES_OK) {
//        Acc_Update(&acc_data);
//      }
//
//      /* Get free fall status */
//      GetFreeFallStatus();
//    }
//  }
//
//  /* Free fall notification */
//  if(request_free_fall_notify == TRUE) {
//    request_free_fall_notify = FALSE;
//    Free_Fall_Notify();
//  }
//}
