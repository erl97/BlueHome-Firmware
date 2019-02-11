/*
 * source.h
 *
 *  Created on: 08.01.2019
 *      Author: dinkelsv64505
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#include "stdint.h"
#include "RP_Types.h"
#include "RP_Init.h"

//SourceTypes
#define SOURCETYPE_NOSOURCE 0
#define SOURCETYPE_BLUETOOTH 1
#define SOURCETYPE_RTC 2
#define SOURCETYPE_BUTTON 3
#define SOURCETYPE_SENSOR 4

//How-To-Compare(Rules)
#define COMPARE_NOT_IMPORTANT 0
#define COMPARE_EQUALS 1
#define COMPARE_GREATER 2
#define COMPARE_SMALLER 3

Source sourceBuffer[SIZEOFSOURCEBUFFER];
volatile uint8_t write_idx_source;

//zu Programmstart leerer rules-Array
Rule myRules[SIZEOFMYRULES];


//funktionen
//source.handler
void rp_rc_init();

void source_handler(Source source);


#endif /* SOURCE_H_ */
