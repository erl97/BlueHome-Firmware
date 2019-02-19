/*
 * SAM_TouchButton.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "SourceActionManager/SAM_TouchButton.h"

#include "SourceActionManager/SAM_Init.h"
#include "HardwareUtil/HW_Init.h"

void sam_tb_init()
{

	SAM_PIN_TB_INT = 0xffff;
	hw_init_registerPinIdentfier(SAM_ID_TOUCHBUTTON, &SAM_PIN_TB_INT);

}

void sam_tb_notifyEvent()
{

}
