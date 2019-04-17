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
#include "SourceActionManager/SAM_Relais.h"
#include "SourceActionManager/SAM_Pieper.h"

void sam_init_init()
{

	sam_bl_init();
	sam_rtc_init();
	sam_tb_init();
	sam_prog_init();
	sam_relay_init();
	sam_pieper_init();
}

void sam_init_initHW()
{
	sam_pieper_initHW();
	sam_tb_initHW();

}
