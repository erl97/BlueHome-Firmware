/*
 * HW_MAC.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HW_MAC.h"
#include "HW_Memory.h"

#include "BlueNRG1_flash.h"

void hw_mac_init()
{
	//ein Schleifendurchlauf entspricht einer MAC-Adresse
	// MAC-ID 0 is own MAC
	for (int i = 0; i < SIZEOFMAC; i++)
	{
		MAC_Addr[i][0] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 6) + 0);
		MAC_Addr[i][1] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 6) + 1);
		MAC_Addr[i][2] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 6) + 2);
		MAC_Addr[i][3] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 6) + 3);
		MAC_Addr[i][4] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 6) + 4);
		MAC_Addr[i][5] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 6) + 5);
	}
}
