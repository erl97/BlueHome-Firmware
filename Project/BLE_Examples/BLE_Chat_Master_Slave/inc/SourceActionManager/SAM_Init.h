/*
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
	SAM_ID_BUTTON = 5,
	SAM_ID_PWM = 6,
	SAM_ID_RELAY = 7,
	SAM_ID_DIO = 8,
	SAM_NUM
};

#define SAM_VALUE_ID_GetPwm0	70


//PINS -> INT SOURCE PIN ID = SAM IDs
// Additional Pins here //
//#define SAM_PIN_ID_XXX	100

void sam_init_init();

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_INIT_H_ */
