/*
 * HW_MAC.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_MAC_H_
#define BLUEHOME_HARDWAREUTIL_HW_MAC_H_

#include <stdint.h>

#include "RuleProcess/RP_Init.h"

//zu Programmstart leerer MAC-Array
uint8_t MAC_Addr[SIZEOF_MAC][6];

uint8_t MAC_UNINITIALIZED;

void hw_mac_init();

uint8_t hw_mac_getMacId(uint8_t macAddr[6]);
void hw_mac_getMac(uint8_t id, uint8_t* macAddr);

uint8_t hw_mac_compareMac(uint8_t macAddr1[6], uint8_t macAddr2[6]);
void hw_mac_writeCurrentMacToFlash();
void hw_mac_writeMacToFlash(uint8_t *b_addr, uint8_t id);
void hw_mac_generateDeviceName();
void hw_mac_generateDeviceAddress(uint8_t *addr);


#endif /* BLUEHOME_HARDWAREUTIL_HW_MAC_H_ */
