/*
 * DB_TestCases.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "Debug/DB_TestCases.h"

#include <stdint.h>

#include "SDK_EVAL_Led.h"
#include "SDK_EVAL_Button.h"
#include "SDK_EVAL_Config.h"

#include "Debug/DB_Console.h"

#include "BlueNRG1_gpio.h"
#include "misc.h"

void db_tc_init_Eval()
{
	SdkEvalIdentification();
//
//	SdkEvalLedInit(LED1);
//	SdkEvalLedInit(LED2);
//	SdkEvalLedInit(LED3);
//
//	SdkEvalLedOff(LED1);
//	SdkEvalLedOff(LED2);
//	SdkEvalLedOff(LED3);

	SdkEvalComUartInit(115200);
	SdkEvalComUartIrqConfig(ENABLE);

	db_cs_printString("Welcome to BlueHome Node !\r");
	//db_cs_printString(">");

//	char message[] =
//	{ 'B', 'l', 'u', 'e', ' ', 'H', 'o', 'm', 'e', '\r', '\0' };
//
//	for (int i = 0; i < sizeof(message) / sizeof(char); i++)
//	{
//		SdkEvalComIOSendData((uint8_t) message[i]);
//	}


}


void db_tc_init_GPIO_Int()
{

	//SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Get_ButtonGpioPin(BUTTON_1);
	GPIO_InitStructure.GPIO_Mode = GPIO_Input;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);
//
//	/* Set the GPIO interrupt priority and enable it */
	NVIC_InitType NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//
//	/* Configures EXTI line for BUTTON_1 */
	GPIO_EXTIConfigType GPIO_EXTIStructure;
	GPIO_EXTIStructure.GPIO_Pin = Get_ButtonGpioPin(BUTTON_1);
	GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Edge;
	GPIO_EXTIStructure.GPIO_Event = IRQ_ON_RISING_EDGE;
	GPIO_EXTIConfig(&GPIO_EXTIStructure);
//
//	/* Clear pending interrupt */
//	GPIO_ClearITPendingBit(Get_ButtonGpioPin(BUTTON_1));
//
//	/* Enable the interrupt */
//	GPIO_EXTICmd(Get_ButtonGpioPin(BUTTON_1), ENABLE);

	//Relais
//PIN_2

}

void db_tc_test_EvalLeds()
{
	for (int i = 0; i < 10; i++)
	{
		SdkEvalLedToggle(LED3);
		for (uint32_t j = 0; j < 1000000; j++)
		{
		__NOP();
		}
	}
}

void db_tc_GPIO_Int()
{
	SdkEvalLedToggle(LED3);

	if (GPIO_GetITPendingBit(Get_ButtonGpioPin(BUTTON_1)) == SET)
	{
		GPIO_ClearITPendingBit(Get_ButtonGpioPin(BUTTON_1));
	}
}
