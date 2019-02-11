/*
 * HW_Memory.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_
#define BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_

//Memory-Map-Addition; analog to system_bluenrg2.h

//for 100 actions a 27B: 2700B -> 0xA8C; 0xB00 -> 2816 (2,75KB)
#define _MEMORY_ACTIONS_BEGIN_  0x1007F500
#define _MEMORY_ACTIONS_SIZE_   0xB00          	/* 2816B */
#define _MEMORY_ACTIONS_END_    0x1007FFFF

//for 100 rules a 52B: 5200B -> 0x1450; 0x2000 -> 8192 (8KB)
#define _MEMORY_RULES_BEGIN_  0x1007D500
#define _MEMORY_RULES_SIZE_   0x2000          	/* 8192B */
#define _MEMORY_RULES_END_    0x1007F4FF

//for 50 MACs a 48B: 2400B -> 0x960; 0xA00 -> 2560 (2,5KB)
#define _MEMORY_MAC_BEGIN_  0x1007CB00
#define _MEMORY_MAC_SIZE_   0xA00          		/* 2560B */
#define _MEMORY_MAC_END_    0x1007D4FF



//Speicher-defines in system_bluenrg2.h �ndern?


#endif /* BLUEHOME_HARDWAREUTIL_HW_MEMORY_H_ */
