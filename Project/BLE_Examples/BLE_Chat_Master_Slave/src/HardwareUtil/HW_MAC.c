/*
 * HW_MAC.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "HardwareUtil/HW_MAC.h"
#include "HardwareUtil/HW_Memory.h"
#include "HardwareUtil/HW_Init.h"

#include "BlueNRG1_flash.h"
#include "ble_status.h"

#include "Debug/DB_Console.h"
#include "Debug/DB_Assert.h"


uint8_t defaultAddr[6] = {0x00, 0x00, 0x00, 0x42, 0x42, 0x42};
uint8_t DEVICE_BDADDR[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t DEVICE_NAME[16];


void hw_mac_init()
{

	for(int i = 0; i < 16; i++){
		DEVICE_NAME[i] = 'X';
	}

	// MAC-ID 0 is own MAC
	for (int i = 0; i < SIZEOF_MAC; i++)
	{
		MAC_Addr[i][0] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 8) + 0);
		MAC_Addr[i][1] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 8) + 1);
		MAC_Addr[i][2] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 8) + 2);
		MAC_Addr[i][3] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 8) + 3);
		MAC_Addr[i][4] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 8) + 4);
		MAC_Addr[i][5] = FLASH_ReadByte(_MEMORY_MAC_BEGIN_ + (i * 8) + 5);
	}

	// Set own MAC
	uint8_t uninitAddr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	if(hw_mac_compareMac(MAC_Addr[0], uninitAddr)){ // No valid MAC -> generate new
		for(int i = 0; i < 6; i++){
			DEVICE_BDADDR[i] = defaultAddr[i];
		}
		MAC_UNINITIALIZED = 1;
		db_cs_printString("MAC Unitialized -> Scanning MACs...\r");

	}else{ // Set own MAC address
		for(int i = 0; i < 6; i++){
			DEVICE_BDADDR[i] = MAC_Addr[0][i];
		}
		MAC_UNINITIALIZED = 0;

	}

	//Set name
	hw_mac_generateDeviceName();
}

void hw_mac_writeCurrentMacToFlash(){
	hw_mac_writeMacToFlash(DEVICE_BDADDR, 0);
}

void hw_mac_writeMacToFlash(uint8_t *b_addr, uint8_t id){
	db_cs_printString("Write MAC to Flash...\r");
	uint8_t addr[8];
	for(int i = 0; i < 6; i++){
		addr[i] = b_addr[i];
		MAC_Addr[id][i] = b_addr[i];
	}

	//Can only write 32 bit - alignment
	addr[6] = 0;
	addr[7] = 0;

	uint32_t temp = (addr[3] << 24) | (addr[2] << 16) | (addr[1] << 8) | (addr[0] << 0);
	FLASH_ProgramWord(_MEMORY_MAC_BEGIN_+ (id*8), temp);
	temp = (addr[7] << 24) | (addr[6] << 16) | (addr[5] << 8) | (addr[4] << 0);
	FLASH_ProgramWord(_MEMORY_MAC_BEGIN_ + (id*8) + 4, temp);
}

void hw_mac_generateDeviceName(){

	DEVICE_NAME[0] = 'b';
	DEVICE_NAME[1] = 'l';
	DEVICE_NAME[2] = 'h';
	DEVICE_NAME[3] = 'n';
	DEVICE_NAME[4] = 't';

	//Write HW ID into Name
	char nt[2];
	db_cs_itoa(HW_ID, nt);

	if(HW_ID < 10){
		DEVICE_NAME[5] = '0';
		DEVICE_NAME[6] = nt[0];
	}else {
		DEVICE_NAME[5] = nt[0];
		DEVICE_NAME[6] = nt[1];
	}


	DEVICE_NAME[7] = 'u';
	DEVICE_NAME[8] = 'i';

	//Write MAC into Name
	for(int i = 2; i >= 0; i--){
		uint8_t temp = DEVICE_BDADDR[i];

		DEVICE_NAME[13-2*i] = TO_HEX(((temp & 0xF0) >> 4));
		DEVICE_NAME[14-2*i] = TO_HEX((temp & 0x0F));
	}

	DEVICE_NAME[15] ='d';
}

void hw_mac_generateDeviceAddress(uint8_t *addr)
{

  uint8_t random_number[8];

  /* get a random number from BlueNRG */
  tBleStatus ret = hci_le_rand(random_number);
  if(ret != BLE_STATUS_SUCCESS){
	  db_as_assert(DB_AS_ERROR_BLUETOOTH, "Random number generator failed");
  }

  /* Setup last 3 bytes of public address with random number */
  addr[0] = (uint8_t) (random_number[0]);
  addr[1] = (uint8_t) (random_number[3]);
  addr[2] = (uint8_t) (random_number[6]);
  addr[3] = 0xE1;
  addr[4] = 0x80;
  addr[5] = 0x02;

}

uint8_t hw_mac_getMacId(uint8_t macAddr[6]){


	for (int i = 0; i < SIZEOF_MAC; i++){
		if(hw_mac_compareMac(macAddr, MAC_Addr[i])){
			return i;
		}
	}

	db_as_assert(DB_AS_ERROR_BLUETOOTH, "Unknown MAC !");
	return 0xff;
}

uint8_t hw_mac_compareMac(uint8_t macAddr1[6], uint8_t macAddr2[6]){
	if( macAddr1[0] == macAddr2[0] &&
		macAddr1[1] == macAddr2[1] &&
		macAddr1[2] == macAddr2[2] &&
		macAddr1[3] == macAddr2[3] &&
		macAddr1[4] == macAddr2[4] &&
		macAddr1[5] == macAddr2[5]){
		return 1;
	}else return 0;
}

void hw_mac_getMac(uint8_t id, uint8_t* macAddr){
	if(id > SIZEOF_MAC){
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
