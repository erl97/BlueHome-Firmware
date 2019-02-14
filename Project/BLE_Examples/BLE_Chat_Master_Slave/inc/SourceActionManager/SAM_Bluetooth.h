/*
 * SAM_Bluetooth.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_SOURCEACTIONMANAGER_SAM_BLUETOOTH_H_
#define BLUEHOME_SOURCEACTIONMANAGER_SAM_BLUETOOTH_H_

#include "RuleProcess/RP_Types.h"

void sam_bl_init();

void sam_bl_notifyEvent(uint8_t attr_data_length, uint8_t *attr_data);

void sam_bl_triggerAction(Action action);

#endif /* BLUEHOME_SOURCEACTIONMANAGER_SAM_BLUETOOTH_H_ */
