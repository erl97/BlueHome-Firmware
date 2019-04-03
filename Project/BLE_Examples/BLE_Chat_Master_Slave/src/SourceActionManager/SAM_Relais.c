/*
 * RelaisManager.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "SourceActionManager/SAM_RELAIS.h"

#include <stdint.h>

#include "BlueNRG1_gpio.h"

#include "SourceActionManager/SAM_Init.h"
#include "HardwareUtil/HW_Init.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_ActionManager.h"

#include "Debug/DB_Console.h"

void sam_relay_init()
{

	SAM_PIN_RELAIS_0 = 0xffff; //Define as output
	SAM_PIN_RELAIS_1 = 0xffff; //Define as output
	SAM_PIN_RELAIS_2 = 0xffff; //Define as output
	SAM_PIN_RELAIS_3 = 0xffff; //Define as output


	// Register Pin
	hw_init_registerPinIdentfier(SAM_PIN_ID_RELAIS_0, &SAM_PIN_RELAIS_0);
	hw_init_registerPinIdentfier(SAM_PIN_ID_RELAIS_1, &SAM_PIN_RELAIS_1);
	hw_init_registerPinIdentfier(SAM_PIN_ID_RELAIS_2, &SAM_PIN_RELAIS_2);
	hw_init_registerPinIdentfier(SAM_PIN_ID_RELAIS_3, &SAM_PIN_RELAIS_3);

	// Register Source
	rp_sm_registerSAMSourceIdentfier(SAM_ID_RELAY, sam_relay_triggerSource);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_RELAY, sam_relay_triggerAction);

	// Register ValueFct

}

void sam_relay_triggerSource(uint8_t paramLen, uint8_t *param)
{
	db_cs_printString("Relay Source\r");
	//Get Data
	//Create Source
	//Add to buffer
}

void sam_relay_triggerAction(Action *action)
{
	db_cs_printString("Relay Action\r");

	switch(action->actionID){
		case SAM_RELAY_ACT_ID_TOGGLE:{

			uint8_t relayNum = action->param[0];
			db_cs_printString("Toggle Relais Nr: ");
			db_cs_printInt(relayNum);
			db_cs_printString("\r");

			switch(relayNum){
				case 0: if(SAM_PIN_RELAIS_0 != 0xffff) GPIO_ToggleBits(SAM_PIN_RELAIS_0); break;
				case 1: if(SAM_PIN_RELAIS_1 != 0xffff) GPIO_ToggleBits(SAM_PIN_RELAIS_1); break;
				case 2: if(SAM_PIN_RELAIS_2 != 0xffff) GPIO_ToggleBits(SAM_PIN_RELAIS_2); break;
				case 3: if(SAM_PIN_RELAIS_3 != 0xffff) GPIO_ToggleBits(SAM_PIN_RELAIS_3); break;
			}

			break;

		}
		case SAM_RELAY_ACT_ID_ON:{

			uint8_t relayNum = action->param[0];
			db_cs_printString("Enable Relais Nr: ");
			db_cs_printInt(relayNum);
			db_cs_printString("\r");

			switch(relayNum){
				case 0: if(SAM_PIN_RELAIS_0 != 0xffff) GPIO_SetBits(SAM_PIN_RELAIS_0); break;
				case 1: if(SAM_PIN_RELAIS_1 != 0xffff) GPIO_SetBits(SAM_PIN_RELAIS_1); break;
				case 2: if(SAM_PIN_RELAIS_2 != 0xffff) GPIO_SetBits(SAM_PIN_RELAIS_2); break;
				case 3: if(SAM_PIN_RELAIS_3 != 0xffff) GPIO_SetBits(SAM_PIN_RELAIS_3); break;
			}

			break;
		}
		case SAM_RELAY_ACT_ID_OFF:{

			uint8_t relayNum = action->param[0];
			db_cs_printString("Disable Relais Nr: ");
			db_cs_printInt(relayNum);
			db_cs_printString("\r");

			switch(relayNum){
				case 0: if(SAM_PIN_RELAIS_0 != 0xffff) GPIO_ResetBits(SAM_PIN_RELAIS_0); break;
				case 1: if(SAM_PIN_RELAIS_1 != 0xffff) GPIO_ResetBits(SAM_PIN_RELAIS_1); break;
				case 2: if(SAM_PIN_RELAIS_2 != 0xffff) GPIO_ResetBits(SAM_PIN_RELAIS_2); break;
				case 3: if(SAM_PIN_RELAIS_3 != 0xffff) GPIO_ResetBits(SAM_PIN_RELAIS_3); break;
			}

			break;
		}
	}

}
