/*
 * SAM_Pieper.c
 *
 *  Created on: 05.04.2019
 *      Author: Oliver
 */


#include <stdint.h>

#include "BlueNRG1_mft.h"
#include "BlueNRG2.h"
#include "BlueNRG1_sysCtrl.h"
#include "misc.h"

#include "SourceActionManager/SAM_Init.h"
#include "SourceActionManager/SAM_Pieper.h"
#include "HardwareUtil/HW_Init.h"
#include "RuleProcess/RP_ActionManager.h"
#include "RuleProcess/RP_SourceManager.h"

#include "Debug/DB_Console.h"

#include "BlueNRG1_gpio.h"

void sam_pieper_init()
{
	SAM_PIN_PIEPER_0 = 0xfff1; //Define as serial1

	// Register Pin
	hw_init_registerPinIdentfier(SAM_PIN_ID_PIEPER_0, &SAM_PIN_PIEPER_0);

	// Register Action
	rp_am_registerSAMActionIdentfier(SAM_ID_PIEPER, sam_pieper_triggerAction);
}

void sam_pieper_initHW()
{
	if(SAM_PIN_PIEPER_0 < 0xf000){

		SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_MTFX2, ENABLE);

		MFT_InitType mft_init;
		mft_init.MFT_Mode = MFT_MODE_1;
		mft_init.MFT_Clock1 = MFT_NO_CLK;
		mft_init.MFT_Clock2 = MFT_NO_CLK;
		mft_init.MFT_Prescaler = 128;

		// Pitch Settings
		mft_init.MFT_CRA = SAM_PIEPER_DEFAULT_PITCH;
		mft_init.MFT_CRB = SAM_PIEPER_DEFAULT_PITCH;

		MFT_Init(MFT2, &mft_init);
		MFT_Cmd(MFT2, ENABLE);

		MFT_TnXEN(MFT2, MFT_TnA, ENABLE);
		//MFT_TnXEN(MFT2, MFT_TnB, ENABLE);

		/* NVIC configuration */
		NVIC_InitType NVIC_InitStructure;

		/* Enable the MFT2B Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = MFT2B_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		MFT_EnableIT(MFT2, MFT_IT_TND, ENABLE);

	}

}

void sam_pieper_triggerAction(Action *action)
{
	db_cs_printString("Pieper Action\r");

	switch(action->actionID){
		case SAM_PIEPER_ACT_ID_PEEP:{
			MFT2->TNCKC_b.TNC1CSEL = MFT_PRESCALED_CLK;
			if(action->param[0] == 0){
				MFT2->TNCRA = SAM_PIEPER_DEFAULT_PITCH;
				MFT2->TNCRB = SAM_PIEPER_DEFAULT_PITCH;
			}else{
				MFT2->TNCRA = 0x0f * action->param[0];
				MFT2->TNCRB = 0x0f * action->param[0];
			}

			MFT2->TNCNT2 = 0xffff;
			MFT2->TNCKC_b.TNC2CSEL = MFT_PRESCALED_CLK;

			break;
		}
		case SAM_PIEPER_ACT_ID_START:{
			MFT2->TNCKC_b.TNC1CSEL = MFT_PRESCALED_CLK;
			if(action->param[0] == 0){
				MFT2->TNCRA = SAM_PIEPER_DEFAULT_PITCH;
				MFT2->TNCRB = SAM_PIEPER_DEFAULT_PITCH;
			}else{
				MFT2->TNCRA = 0x0f * action->param[0];
				MFT2->TNCRB = 0x0f * action->param[0];
			}
			break;
		}
		case SAM_PIEPER_ACT_ID_STOP:{
			MFT2->TNCKC_b.TNC1CSEL = MFT_NO_CLK;
			break;
		}
	}
}

void sam_pieper_isr(){
	MFT2->TNCKC_b.TNC1CSEL = MFT_NO_CLK;
	MFT2->TNCKC_b.TNC2CSEL = MFT_NO_CLK;
	MFT_ClearIT(MFT2, MFT_IT_TND);
}
