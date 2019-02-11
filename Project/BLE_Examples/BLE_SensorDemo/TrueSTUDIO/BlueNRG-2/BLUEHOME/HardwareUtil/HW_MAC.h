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
uint8_t MAC_Addr[SIZEOFMAC][6];

void hw_mac_init();


#endif /* BLUEHOME_HARDWAREUTIL_HW_MAC_H_ */
