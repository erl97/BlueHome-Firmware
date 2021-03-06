/*
 * HW_UART.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "HardwareUtil/HW_UART.h"

#include <stdint.h>

#include "SDK_EVAL_Com.h"

#include "Debug/DB_Console.h"

void hw_uart_init()
{
	currentRBPos = 0;
}

void hw_uart_receiveISR()
{
	char read_data = 0;

	if (UART_GetITStatus(UART_IT_RX) != RESET)
	{

		while (!UART_GetFlagStatus(UART_FLAG_RXFE))
		{

			/* Read byte from the receive FIFO */
			read_data = (char) (UART_ReceiveData() & 0xFF);

		}
	}

	//hw_uart_sendChar(read_data); // TEST ECHO

	if (read_data == '\r')
	{
		receiveBuffer[currentRBPos] = '\0';
		currentRBPos = 0;

		//Call receive func
		db_cs_executeCommand(receiveBuffer);
	}
	else
	{
		receiveBuffer[currentRBPos] = read_data;
		currentRBPos++;
	}

}

void hw_uart_sendString(char *string)
{
	char *currChar = string;
	while (*currChar != '\0')
	{
		hw_uart_sendChar(*currChar);
		currChar++;
	}
}

void hw_uart_sendChar(char c)
{
	while (UART_GetFlagStatus(UART_FLAG_TXFF) == SET)
	{
	};


	/* send the data */
	UART_SendData((uint16_t) c);
}
