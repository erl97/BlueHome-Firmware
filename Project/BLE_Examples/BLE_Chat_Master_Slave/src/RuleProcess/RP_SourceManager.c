/*
 * RP_InterruptManager.c
 *
 *  Created on: 12.02.2019
 *      Author: dinkelsv64505
 */

#include <bluenrg1_stack.h>
#include <BlueNRG1_gpio.h>
#include <BlueNRG1_i2c.h>
#include <BlueNRG1_spi.h>
#include <BlueNRG2.h>
#include <clock.h>
#include <Debug/DB_Console.h>
#include <hal_types.h>
#include <HardwareUtil/HW_I2C.h>
#include <HardwareUtil/HW_Init.h>
#include <HardwareUtil/HW_SPI.h>
#include <HardwareUtil/HW_UART.h>
#include <lsm6ds3.h>
#include <RuleProcess/RP_SourceManager.h>
#include <sys/_stdint.h>
#include <SourceActionManager/SAM_Pieper.h>

extern LSM6DS3_DrvExtTypeDef *Imu6AxesDrvExt;

uint8_t volatile PROGRAM_FLAG = 0;

void rp_sm_init()
{
	for(uint8_t i = 0; i < SAM_NUM; i++){
		sourceFlags[i] = 0;
	}
}

void rp_sm_tick(){
	for(uint8_t i = 0; i < SAM_NUM; i++){
		if(sourceFlags[i] == 1){
			sourceFlags[i] = 0;
			//Call Source Trigger
			rp_sm_triggerSource(i, 0, NULL);
		}
	}
}

void rp_sm_setSourceFlag(uint8_t samId){
	sourceFlags[samId] = 1;
}

uint8_t rp_sm_registerSAMSourceIdentfier(uint8_t samId, SamSource_Fct fct){
	sourceFct[samId] = fct;
	return 0;
}

void rp_sm_triggerSource(uint8_t samId, uint8_t paramLen, uint8_t *param){
	db_cs_printString("Trigger Source : ");

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
	db_cs_printString("GPIO Int ");
//    db_cs_printInt(GPIO->MIS);
//    db_cs_printString(" ");
//    db_cs_printInt(GPIO->RIS);
//    db_cs_printString("\r");

	// Determine Triggered Source
	for(int i = 1; i < PIN_NUM; i++){
		uint32_t pinAddr = hw_init_getPinFromPinId(i);

		if(pinAddr <= 0xfff0 && pinAddr != 0x0000){
				uint8_t samId = hw_init_getSamIdFromPinId(i);
//				db_cs_printInt(samId);
//				db_cs_printString(" ");
//				db_cs_printInt(pinAddr);
//				db_cs_printString(" - ");
				if(samId != SAM_ID_UNKNWON){
					if(GPIO_GetITPendingBit(pinAddr) == SET){
						GPIO_ClearITPendingBit(pinAddr);

						if(PROGRAM_FLAG) PROGRAM_FLAG = 0;
						else rp_sm_setSourceFlag(samId);

						break;
					}
				}
		}
	}
	db_cs_printString("\r");

}


void MFT2B_Handler(void){
	sam_pieper_isr();
}

void I2C2_Handler(void){

//	db_cs_printString("I2C Interrupt\r");
//	db_cs_printInt(I2C2->MISR);
//	db_cs_printInt(I2C_GetITPendingBit(I2C2, I2C_IT_TXFE));
//	db_cs_printString(" ");
//	db_cs_printInt(I2C_GetITPendingBit(I2C2, I2C_IT_MTD));
//	db_cs_printString(" ");
//	db_cs_printInt(I2C_GetITPendingBit(I2C2, I2C_IT_MTDWS));

//	db_cs_printString("\r");
	if(I2C_GetITPendingBit(I2C2, I2C_IT_MTD) || I2C_GetITPendingBit(I2C2, I2C_IT_MTDWS)){
		hw_i2c_isr_bufferEmpty();
		I2C_ClearITPendingBit(I2C2, I2C_IT_MTD);
		I2C_ClearITPendingBit(I2C2, I2C_IT_MTDWS);
	}

	if(I2C_GetITPendingBit(I2C2, I2C_IT_LBR)){
		hw_i2c_isr_received();
		I2C_ClearITPendingBit(I2C2, I2C_IT_LBR);
	}

}

void SPI_Handler(void) {

//	db_cs_printString("SPI Interrupt\r");
//	db_cs_printInt(SPI->RIS);
//	db_cs_printString("\r");

	hw_spi_isp();
	SPI_ClearITPendingBit(SPI_IT_TE);

}

