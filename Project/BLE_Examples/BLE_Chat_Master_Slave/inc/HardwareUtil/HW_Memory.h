/*
 * HW_Memory.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_
#define BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_

#include "BlueNRG1_flash.h"

//Memory-Map-Addition; analog to system_bluenrg2.h

//HARDWARE CONFIG: 128 Byte -> 0x80
//1 Byte HW_ID (node#1/#2/...), 1 Byte HW_Version, 1 Byte number bus devices, + 13 Byte alignment; 0x1007ef80
//14 Byte BNRG_GPIO, + 2 Byte alignment; 0x1007ef90
//16 Byte Expander_GPIO; 0x1007efa0
//64 = 2 * 32 Byte bus addresses; 0x1007efb0
#define _MEMORY_HWCONFIG_BEGIN_	0x1007EF80
#define	_MEMORY_HWCONFIG_SIZE_	0x80
#define _MEMORY_HWCONFIG_END_	0X1007EFFF

//for 32 actions a 27 Byte: 864 Byte
#define _MEMORY_ACTIONS_PAGE       (N_PAGES-4)
#define _MEMORY_ACTIONS_ADDR       ((_MEMORY_ACTIONS_PAGE*N_BYTES_PAGE) + FLASH_START)

//for 32 rules a 52 Byte: 1664 Byte
#define _MEMORY_RULES_PAGE         (N_PAGES-6)
#define _MEMORY_RULES_ADDR         ((_MEMORY_RULES_PAGE*N_BYTES_PAGE) + FLASH_START)

//for 32 MACs a 8 Byte: 256 Byte
#define _MEMORY_MAC_PAGE       	   (N_PAGES-8)
#define _MEMORY_MAC_ADDR           ((_MEMORY_MAC_PAGE*N_BYTES_PAGE) + FLASH_START)

//Speicher-defines in system_bluenrg2.h ändern?


#endif /* BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_ */
