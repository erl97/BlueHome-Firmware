/*
 * SAM_TouchButton.h
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_TOUCHBUTTON_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_TOUCHBUTTON_H_

#include <stdint.h>

uint32_t SAM_PIN_TB_INT;

void sam_tb_init();

void sam_tb_notifyEvent();

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_TOUCHBUTTON_H_ */
