/*
 * RP_InterruptManager.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include "RuleProcess/RP_SourceManager.h"

#include "Bluetooth/BL_gatt_db.h"

#include "BlueNRG_x_device.h"
#include "BlueNRG1_gpio.h"
#include "bluenrg1_stack.h"
#include "clock.h"
#include "BlueNRG1_i2c.h"

#include "ble_const.h"
//#include "SDK_EVAL_Com.h"

#include "HardwareUtil/HW_UART.h"
#include "HardwareUtil/HW_I2C.h"

#include "Debug/DB_TestCases.h"
#include "Debug/DB_Console.h"

#include "SDK_EVAL_Led.h"

#include "lsm6ds3.h"
#include "lsm6ds3_hal.h"

#include "SourceActionManager/SAM_Init.h"
#include "SourceActionManager/SAM_Pieper.h"


extern LSM6DS3_DrvExtTypeDef *Imu6AxesDrvExt;

void rp_im_init()
{

}

uint8_t rp_sm_registerSAMSourceIdentfier(uint8_t samId, SamSource_Fct fct){
	sourceFct[samId] = fct;
	return 0;
}

void rp_sm_triggerSource(uint8_t samId, uint8_t paramLen, uint8_t *param){
	db_cs_printString("Trigger Source :\r");

	sourceFct[samId](paramLen, param);
}

/*
 * Bluetooth ISR
 */
void Blue_Handler(void)
{
	RAL_Isr();
}

void SysTick_Handler(void)
{
	SysCount_Handler();
}

/*
 * UART ISR
 */
void UART_Handler(void)
{
	hw_uart_receiveISR();
}

/*
 * GPIO
 */
void GPIO_Handler(void)
{
	// DEBUG //
	//db_tc_GPIO_Int();

	// Determine Triggered Source
	for(int i = 0; i < SAM_NUM; i++){
		uint32_t pinAddr = hw_init_getIntPinFromSAM(i);
		if(pinAddr != 0xffff && GPIO_GetITPendingBit(pinAddr) == SET){
			GPIO_ClearITPendingBit(pinAddr);

			//Call Source Trigger
			rp_sm_triggerSource(i, 0, NULL);
		}
	}

}


void MFT2B_Handler(void){
	sam_pieper_isr();
}

void I2C2_Handler(void){

	if(I2C_GetITPendingBit(I2C2, I2C_IT_TXFE)){
		hw_i2c_isr_bufferEmpty();
		I2C_ClearITPendingBit(I2C2, I2C_IT_TXFE);
	}

}

