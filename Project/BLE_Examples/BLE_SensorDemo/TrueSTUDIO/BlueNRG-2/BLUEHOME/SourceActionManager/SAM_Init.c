/*
 * SAM_Init.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "SAM_Init.h"

#include "SAM_RTC.h"
#include "SAM_TouchButton.h"
#include "SAM_Bluetooth.h"

void sam_init_init()
{

	sam_bl_init();
	sam_rtc_init();
	sam_tb_init();

}
