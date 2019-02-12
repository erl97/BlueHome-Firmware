/*
 * HW_UART.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "HW_UART.h"

#include "SDK_EVAL_Com.h"

void hw_uart_receiveISR()
{
	uint8_t read_data = 0;

	if (UART_GetITStatus(UART_IT_RX) != RESET)
	{

		while (!UART_GetFlagStatus(UART_FLAG_RXFE))
		{

			/* Read byte from the receive FIFO */
			read_data = (uint8_t) (UART_ReceiveData() & 0xFF);

		}
	}

	//TODO Call Console Function with string
}
