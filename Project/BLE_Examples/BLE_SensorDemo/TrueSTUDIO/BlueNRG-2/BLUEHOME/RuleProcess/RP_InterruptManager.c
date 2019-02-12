/*
 * RP_InterruptManager.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "RP_InterruptManager.h"

#include "BlueNRG_x_device.h"

#include "SourceActionManager/SAM_Bluetooth.h"
#include "HardwareUtil/HW_UART.h"
#include "Debug/DB_TestCases.h"

#include "SDK_EVAL_Led.h"

// INTERRUPT SOURCES //
#include "SourceActionManager/SAM_RTC.h"
#include "SourceActionManager/SAM_TouchButton.h"

void rp_im_init()
{

}

/*
 * Bluetooth ISR
 */
void Blue_Handler(void)
{
	sam_bl_notifyEvent();
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
	// DEBUG db_tc_GPIO_Int();

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
