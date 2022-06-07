/*
 * Delays.c
 *
 *  Created on: Mar 14, 2022
 *      Author: 77FA9F4-Desktop
 */

#include "Delays.h"
#include "stm32f4xx_hal.h"
/**
 * @brief  Delays for amount of micro seconds
 * @param  micros: Number of microseconds for delay
 * @retval None
 */
void Delay_us(__IO uint32_t micros) {
//#if !defined(STM32F0xx)
//    uint32_t start = DWT->CYCCNT;
//    /* Go to number of cycles for system */
//    micros *= (HAL_RCC_GetHCLKFreq() / 1000000);
//    /* Delay till end */
//    while ((DWT->CYCCNT - start) < micros);
//#else
    /* Go to clock cycles */
//    micros *= (SystemCoreClock / 1000000) / 5;
    micros *= ((SystemCoreClock / 1000000) / 8);
    /* Wait till done */
    do {
//    	   __ASM volatile ("NOP");
    }     while (micros--);
//#endif
}
