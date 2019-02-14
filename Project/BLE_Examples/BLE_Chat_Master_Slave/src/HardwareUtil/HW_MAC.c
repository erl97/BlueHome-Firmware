/*
 * HW_MAC.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HardwareUtil/HW_MAC.h"
#include "HardwareUtil/HW_Memory.h"

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

uint8_t hw_mac_getMacId(uint8_t macAddr[6]){


	for (int i = 0; i < SIZEOFMAC; i++){
		if( macAddr[0] == MAC_Addr[i][0] &&
			macAddr[1] == MAC_Addr[i][1] &&
			macAddr[2] == MAC_Addr[i][2] &&
			macAddr[3] == MAC_Addr[i][3] &&
			macAddr[4] == MAC_Addr[i][4] &&
			macAddr[5] == MAC_Addr[i][5]){
			return i;
		}
	}

	return 0xff;
}

void hw_mac_getMac(uint8_t id, uint8_t* macAddr){
	if(id > SIZEOFMAC){
		macAddr[0] = 0x00;
		macAddr[1] = 0x00;
		macAddr[2] = 0x00;
		macAddr[3] = 0x00;
		macAddr[4] = 0x00;
		macAddr[5] = 0x00;
	}else{
		macAddr[0] = MAC_Addr[id][0];
		macAddr[1] = MAC_Addr[id][1];
		macAddr[2] = MAC_Addr[id][2];
		macAddr[3] = MAC_Addr[id][3];
		macAddr[4] = MAC_Addr[id][4];
		macAddr[5] = MAC_Addr[id][5];
	}
}
