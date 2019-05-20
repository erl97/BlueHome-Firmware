/*
 * HW_Memory.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_
#define BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_

//Memory-Map-Addition; analog to system_bluenrg2.h

//Hardware config: 128 Byte -> 0x80
//1 Byte HW_ID (node#1/#2/...), 1 Byte HW_Version, 1 Byte number bus devices, + 13 Byte alignment; 0x1007ef80
//14 Byte BNRG_GPIO, + 2 Byte alignment; 0x1007ef90
//16 Byte Expander_GPIO; 0x1007efa0
//64 = 2 * 32 Byte bus addresses; 0x1007efb0

#define _MEMORY_HWCONFIG_BEGIN_	0x1007EF80
#define	_MEMORY_HWCONFIG_SIZE_	0x80
#define _MEMORY_HWCONFIG_END_	0X1007EFFF

//for 100 actions a 27B: 2700B -> 0xA8C; 0xB00 -> 2816 (2,75KB)
#define _MEMORY_ACTIONS_BEGIN_  0x1007E480
#define _MEMORY_ACTIONS_SIZE_   0xB00          	/* 2816B */
#define _MEMORY_ACTIONS_END_    0x1007EF7F

//for 100 rules a 52B: 5200B -> 0x1450; 0x2000 -> 8192 (8KB)
#define _MEMORY_RULES_BEGIN_  0x1007C480
#define _MEMORY_RULES_SIZE_   0x2000          	/* 8192B */
#define _MEMORY_RULES_END_    0x1007E47F

//for 50 MACs a 48B: 2400B -> 0x960; 0xA00 -> 2560 (2,5KB)
#define _MEMORY_MAC_BEGIN_  0x1007BA80
#define _MEMORY_MAC_SIZE_   0xA00          		/* 2560B */
#define _MEMORY_MAC_END_    0x1007C480



//Speicher-defines in system_bluenrg2.h ändern?


#endif /* BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_ */
