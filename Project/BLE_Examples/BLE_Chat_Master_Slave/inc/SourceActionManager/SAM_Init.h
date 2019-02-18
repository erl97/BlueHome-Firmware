/*
 * SAM_Init.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_INIT_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_INIT_H_

#define SAM_DEVICE_ID_RTC		1

#define SAM_VALUE_ID_GetPwm0	70

#define SAM_PIN_ID_RTCINT	1
#define SAM_PIN_ID_TBINT	2

void sam_init_init();

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_INIT_H_ */