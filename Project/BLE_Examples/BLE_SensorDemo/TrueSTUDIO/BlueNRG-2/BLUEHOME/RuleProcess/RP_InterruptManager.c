/*
 * RP_InterruptManager.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "RP_InterruptManager.h"

#include "BlueNRG_x_device.h"
#include "bluenrg1_stack.h"
#include "clock.h"

#include "ble_const.h"
//#include "SDK_EVAL_Com.h"

#include "SourceActionManager/SAM_Bluetooth.h"
#include "HardwareUtil/HW_UART.h"
#include "Debug/DB_TestCases.h"

#include "SDK_EVAL_Led.h"

#include "lsm6ds3.h"
#include "lsm6ds3_hal.h"
#include "Demo/gatt_db.h"

// INTERRUPT SOURCES //
#include "SourceActionManager/SAM_RTC.h"
#include "SourceActionManager/SAM_TouchButton.h"

extern LSM6DS3_DrvExtTypeDef *Imu6AxesDrvExt;

void rp_im_init()
{

}

/*
 * Bluetooth ISR
 */
void Blue_Handler(void)
{
	RAL_Isr();
	//sam_bl_notifyEvent();
}

void SysTick_Handler(void)
{
	SysCount_Handler();
}

/*
 * UART ISR
 */
void UART_Handler(void)
{
	hw_uart_receiveISR();
}

/*
 * GPIO
 */
void GPIO_Handler(void)
{
	// DEBUG //
	//db_tc_GPIO_Int();

	uint8_t free_fall_status;
	/* Check if GPIO pin 12 interrupt event occured */
	if (GPIO_GetITPendingBit(LSM6DS3_IRQ_PIN) == SET)
	{
		/* Clear the IRQ pending bit */
		GPIO_ClearITPendingBit(LSM6DS3_IRQ_PIN);

		/* Set the IRQ flag */
		Imu6AxesDrvExt->Get_Status_Free_Fall_Detection(&free_fall_status);
		if (free_fall_status == 1)
		{
			request_free_fall_notify = TRUE;
		}
	}
	//////////////////////

	// RTC INT //
	if (SAM_PIN_RTC_INT != 0xffff
			&& GPIO_GetITPendingBit(Get_ButtonGpioPin(SAM_PIN_RTC_INT)) == SET)
	{
		GPIO_ClearITPendingBit(Get_ButtonGpioPin(SAM_PIN_RTC_INT));
		sam_rtc_notifyEvent();

	}
	// TOUCHBUTTON INT //
	else if (SAM_PIN_TB_INT != 0xffff
			&& GPIO_GetITPendingBit(Get_ButtonGpioPin(SAM_PIN_TB_INT)) == SET)
	{
		GPIO_ClearITPendingBit(Get_ButtonGpioPin(SAM_PIN_TB_INT));
		sam_tb_notifyEvent();
	}
}
