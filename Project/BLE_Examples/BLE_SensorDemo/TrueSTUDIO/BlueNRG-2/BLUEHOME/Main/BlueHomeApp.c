/*
 * BlueHomeApp.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "BlueHomeApp.h"

#include "BlueNRG_x_device.h"

/// DEBUG ///
#include "Debug/DB_TestCases.h"
#include "SDK_EVAL_Led.h"
/////////////

#include "HardwareUtil/HW_Init.h"
#include "SourceActionManager/SAM_Init.h"

int main(void)
{
	SystemInit();

	db_as_init();
	hw_init_init();

	// DEBUG CODE //
	db_tc_init_Eval();
	db_tc_init_GPIO_Int();
	///////////////

	//SAM INITS//
	sam_init_init();

	//HW INIT//
	hw_init_gpio();

	if (db_as_checkInit() == 0)
		SdkEvalLedOn(LED3);



	while (1)
	{
		//do nothing
	}
}

/*******************************************************************************
 * Function Name  : APP_Tick.
 * Description    : Sensor Demo state machine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void APP_Tick(void)
{
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
}
