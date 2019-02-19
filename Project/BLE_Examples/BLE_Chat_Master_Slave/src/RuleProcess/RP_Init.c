/*
 * RuleProcess_Init.c
 *
 *  Created on: 11.02.2019
 *      Author: dinkelsv64505
 */

#include "RuleProcess/RP_Init.h"
#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_SourceManager.h"
#include "RuleProcess/RP_RuleChecker.h"

void rp_init_init()
{
	rp_am_init();
	rp_im_init();
	rp_rc_init();
}
