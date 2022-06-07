/*
 * AD5676.h
 *
 *  Created on: Feb 17, 2022
 *      Author: 77FA9F4-Desktop
 */

#ifndef _AD5676_H_
#define _AD5676_H_

#include "stm32f4xx_hal.h"

#include "main.h"
/* message structure
 * Bit 23(MSB)
 * 4 bit - command
 * 4 bit - address
 * 16 bit - data bits
 * Bit 0 (LSB)
 */
/* message structure */
#define AD5676_OPERATION(addr,cmd) (((addr) & 0x0F) | (((cmd) << 4) & 0xF0))

/* SPI defines for DAC connection */
#define SPI1_SCK_Pin 			GPIO_PIN_5
#define SPI1_SCK_GPIO_Port 		GPIOA
#define SPI1_NLDAC_Pin 			GPIO_PIN_8
#define SPI1_NLDAC_GPIO_Port 	GPIOB
#define SPI1_SYNC_Pin 			GPIO_PIN_9
#define SPI1_SYNC_GPIO_Port 	GPIOC

#define AD5676_HW_GAIN_GPIO_PORT 		GPIOC		//C5
#define AD5676_HW_GAIN_GPIO_PIN			GPIO_PIN_5
#define AD5676_LDAC_GPIO_PORT			GPIOB		//B0
#define AD5676_LDAC_PIN					GPIO_PIN_0
#define AD5676_SYNC_GPIO_PORT			GPIOA		//A4
#define AD5676_SYNC_PIN					GPIO_PIN_4

typedef enum
{
    SEN_ERROR,
    SEN_SUCCESS,
}status_t;


/* command bit definitions */
typedef enum
{
	AD5676_CMD_NOP	= 0x00, 			 /* no operation */
	AD5676_CMD_WRITE,					 /* write to input register */
	AD5676_CMD_UPDATE,					 /* update specific DAC register */
	AD5676_CMD_WRITE_UPDATE,			 /* write and update DAC register */
	AD5676_CMD_POWER,					 /* power up/down */
	AD5676_CMD_LDAC_MASK,				 /* Hardware LDAC mask register */
	AD5676_CMD_SOFT_RESET,				 /* Software reset (power-on reset) */
	AD5676_CMD_INTERNAL_SETUP,   		 /* Gain setup register (LFCSP package only) */
	AD5676_CMD_DAISYCHAIN,				 /* Set up the DCEN register (daisy-chain enable) */
	AD5676_CMD_READBACK,				 /* Set up the readback register (readback enable) */
	AD5676_CMD_UPDATE_CHANNEL_REGISTERS, /* Update all channels of the input register simultaneously with the input data */
	AD5676_CMD_UPDATE_DAC_REGISTERS		 /* Update all channels of the DAC register and input register simultaneously with the input data */

}AD5676_CMD_t;


typedef enum
{
    one_x_Vref,
    two_x_Vref,
}AD5676_SET_GAIN_t;

typedef enum {
    SOFTWARE_RESET,
    HARDWARE_RESET,
    RESET_SELECTED_PIN
}AD5676_RESET_t;

typedef enum {
	onekO_To_GND,
	three_State
}AD5676_PowerOption_t;



status_t AD5676R_init();
status_t AD5676R_write(uint8_t cmd, uint8_t addr, uint16_t val);
status_t AD5676R_read(uint8_t *OutputBuffer);


status_t AD5676_Update_Input_Register_N (uint16_t reg, uint16_t data);
status_t AD5676_Update_DAC_Register_N_from_Input (uint8_t reg);
status_t AD5676_Update_DAC_Channel_N (uint8_t reg, uint16_t data);
status_t AD5676_Update_All_reg_simulat (uint16_t *reg_buff);
status_t AD5676_Read_DAC_Register_N (uint8_t reg, uint8_t *data);
status_t AD5676_Set_DAC_Gain (AD5676_SET_GAIN_t gain);
status_t AD5676_Reset_DAC(AD5676_RESET_t reset);
status_t AD5676_Power_Down_DAC_All();
status_t AD5676_Power_Up_DAC_All();
status_t AD5676_Power_Down_DAC_N (uint16_t dac_num, AD5676_PowerOption_t power_type);

#endif
