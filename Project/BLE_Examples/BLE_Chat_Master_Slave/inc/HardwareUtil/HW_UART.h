/*
 * HW_UART.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_UART_H_
#define BLUEHOME_HARDWAREUTIL_HW_UART_H_

#include <stdint.h>

char receiveBuffer[100];
uint8_t currentRBPos;

void hw_uart_init();

void hw_uart_receiveISR();

void hw_uart_sendString(char *string);
void hw_uart_sendChar(char c);

#endif /* BLUEHOME_HARDWAREUTIL_HW_UART_H_ */
