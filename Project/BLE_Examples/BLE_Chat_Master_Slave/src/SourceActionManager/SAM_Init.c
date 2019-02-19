/*
 * SAM_Init.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */


#include "SourceActionManager/SAM_Init.h"

#include "SourceActionManager/SAM_Bluetooth.h"
#include "SourceActionManager/SAM_RTC.h"
#include "SourceActionManager/SAM_TouchButton.h"
#include "SourceActionManager/SAM_Programming.h"
void sam_init_init()
{

	sam_bl_init();
	sam_rtc_init();
	sam_tb_init();
	sam_prog_init();

}
