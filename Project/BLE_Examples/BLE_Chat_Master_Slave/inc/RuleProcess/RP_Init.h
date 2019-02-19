/*
 * RuleProcess_Init.h
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#ifndef BLUEHOME_RULEPROCESS_RP_INIT_H_
#define BLUEHOME_RULEPROCESS_RP_INIT_H_

// Array-Sizes
// Must fit to memory spaces defined in HW_Memory.h
#define SIZEOF_SOURCEBUFFER			8
#define SIZEOF_ACTIONBUFFER			8

#define MAX_PARAM 19

#define SIZEOFMYRULES				64

#define SIZEOF_MAC					64

#define SIZEOF_PROG_ACTIONS			64


void rp_init_init();

#endif /* BLUEHOME_RULEPROCESS_RP_INIT_H_ */
