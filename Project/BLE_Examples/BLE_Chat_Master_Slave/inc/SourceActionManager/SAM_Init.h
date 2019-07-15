01/*
 * SAM_Init.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_INIT_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_INIT_H_

enum {
	SAM_ID_UNKNWON = 0,
	SAM_ID_BLUETOOTH = 1,
	SAM_ID_RTC = 2,
	SAM_ID_ENVIRONMENT = 3,
	SAM_ID_DISPLAY = 4,
	SAM_ID_TOUCHBUTTON = 5,
	SAM_ID_PWM = 6,
	SAM_ID_RELAY = 7,
	SAM_ID_DIO = 8,
	SAM_ID_PROGRAMMING = 9,
	SAM_ID_PIEPER = 10,
	SAM_ID_LIGHT = 11,
	SAM_NUM
};

enum {
	SAM_VALUE_ID_UNKNWON = 0,
	//1-20 Source Param IDs
	SAM_VALUE_ID_GetPwm0 = 21
};


// Additional Pins here //
enum {
	SAM_PIN_ID_UNKNWON = 0,
	SAM_PIN_ID_RELAIS_0 = 1,
	SAM_PIN_ID_RELAIS_1 = 2,
	SAM_PIN_ID_RELAIS_2 = 3,
	SAM_PIN_ID_RELAIS_3 = 4,
	SAM_PIN_ID_PIEPER_0 = 5,
	SAM_PIN_ID_TB_INT   = 6,
	SAM_PIN_ID_LIGHT_INT = 7,
	SAM_PIN_ID_IO_INT = 8,
	PIN_NUM
};


//#define SAM_PIN_ID_XXX	100

void sam_init_init();
void sam_init_initHW();

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_INIT_H_ */
