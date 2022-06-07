/*
 * AD5676.c
 *
 *  Created on: Feb 17, 2022
 *      Author: 77FA9F4-Desktop
 */


#include "AD5676.h"
#include "stdlib.h"
#include "Delays.h"



/*******************************************************************************
* Function Name		: AD5676R_init
* Description		: Init DAC settings.
* Input			: None
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676R_init(){
	status_t result = SEN_SUCCESS;

    // Reset Sequence
    //HAL_GPIO_WritePin(AD5676_HW_RESET_GPIO_PORT, AD5676_HW_RESET_GPIO_PIN, GPIO_PIN_SET);

    // Keep always high in init to not update the dacs automatically
    HAL_GPIO_WritePin(AD5676_LDAC_GPIO_PORT, AD5676_LDAC_PIN, GPIO_PIN_SET);

    // Set low during send data
    HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    //HAL_GPIO_WritePin(AD5676_HW_RESET_GPIO_PORT, AD5676_HW_RESET_GPIO_PIN, GPIO_PIN_SET);

    return result;
}

/*******************************************************************************
* Function Name		: AD5676R_write
* Description		: SPI interface for writing data to DAC
* Input			: command, address, value
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676R_write(uint8_t cmd, uint8_t addr, uint16_t val){

	status_t result = SEN_SUCCESS;

	uint8_t msgLength = 3;
	uint8_t sendBuffer[msgLength];
	sendBuffer[0] = AD5676_OPERATION(addr,cmd);
	sendBuffer[1] = (val >> 8) & 0x00FF; /* MSB */
	sendBuffer[2] =  val & 0x00FF; /* LSB */


  //  HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_SET);
  //  Delay_us(10);
    HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_RESET);
	Delay_us(1);

    if(HAL_SPI_Transmit(&hspi1, sendBuffer, msgLength,100) != HAL_OK)
    {
    	result = SEN_ERROR;
    }

    HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_SET);
	Delay_us(100);

    return result;
}

/*******************************************************************************
* Function Name		: AD5676_Update_Input_Register_N
* Description		: Update  DAC input register with values from a input data
* Input			: DAC register, data
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Update_Input_Register_N (uint16_t reg, uint16_t data){

	status_t result = SEN_SUCCESS;

	result = AD5676R_write(AD5676_CMD_WRITE,reg, data);

	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Update_DAC_Register_N_from_Input
* Description		: Update  DAC register with values from a input data
* Input			: DAC number, data
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Update_DAC_Register_N_from_Input (uint8_t reg){

	status_t result = SEN_SUCCESS;

	uint16_t data = reg;

	result = AD5676R_write(AD5676_CMD_UPDATE,reg, data);

	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Update_DAC_Channel_N
* Description		: Update  DAC channel with values from a input data. Command
* 						is indipendent than LDAC
* Input			: DAC channel (0..7), data (12 bit(left justif.)/16 bit)
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Update_DAC_Channel_N (uint8_t reg, uint16_t data){

	status_t result = SEN_SUCCESS;

	result = AD5676R_write(AD5676_CMD_WRITE_UPDATE,reg, data);

	return result;

}

/*******************************************************************************
* Function Name		: AD5676_Update_All_reg_simulat
* Description		: Update simultaneously DAC  registers with values from an input buffer
* Input			: Input buffer, holding values for all DACs
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Update_All_reg_simulat (uint16_t *reg_buff){

	uint8_t DAC_size = 8;
	uint8_t i = 0;
	status_t result = SEN_SUCCESS;

	if(reg_buff != NULL)
	{

		// Keep always low, to update output simultaneously
		HAL_GPIO_WritePin(AD5676_LDAC_GPIO_PORT, AD5676_LDAC_PIN, GPIO_PIN_RESET);

		for( i = 0; i < DAC_size; i++)
		{
			result = AD5676R_write(AD5676_CMD_WRITE, i , reg_buff[i]);
		}

		HAL_GPIO_WritePin(AD5676_LDAC_GPIO_PORT, AD5676_LDAC_PIN, GPIO_PIN_SET);

	}
	else
	{
		result = SEN_ERROR;
	}
	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Read_DAC_Register_N
* Description		: Read specific DAC register
* Input			: DAC register, buffer to store the result (data)
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Read_DAC_Register_N (uint8_t reg, uint8_t *data)
{
	status_t result = SEN_SUCCESS;

	if(data != NULL)
	{

		(void)AD5676R_write(AD5676_CMD_READBACK,reg,0x00); // write READ
		(void)AD5676R_write(AD5676_CMD_NOP,0x00,0x00); //write NOP

		HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_RESET);
		Delay_us(10);

		 if(HAL_SPI_Receive(&hspi1, data, 2, 100) != HAL_OK) // maybe read 23 bits?
		  {
			 result = SEN_ERROR;
		  }

		 HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_SET);
	}
	else
	{
		result = SEN_ERROR;
	}

	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Set_DAC_Gain
* Description		: Set gain to be either Vref or 2xVref
* Input			: AD5676_SET_GAIN_t types
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Set_DAC_Gain (AD5676_SET_GAIN_t gain)
{
	status_t result = SEN_SUCCESS;
	uint16_t data = 0;

	if(gain == two_x_Vref)
	{
		data = 1 << 2; // shift 2 times to get eq. to 4
	}

	result = AD5676R_write(AD5676_CMD_INTERNAL_SETUP,0x00, data); /* register is don't care */

	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Reset_DAC
* Description		: Reset DAC, minimum time according to documentation is 80 us
* Input			: AD5676_RESET_t types
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Reset_DAC(AD5676_RESET_t reset)
{
	status_t result = SEN_SUCCESS;

	if(reset == HARDWARE_RESET)
	{
		HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_SET);
		Delay_us(200);	// Need to keep reset active state at least 90us

		HAL_GPIO_WritePin(AD5676_SYNC_GPIO_PORT, AD5676_SYNC_PIN, GPIO_PIN_RESET);
	}
	else if(reset == SOFTWARE_RESET)
	{
		result = AD5676R_write(AD5676_CMD_SOFT_RESET,0x00,0x1234);
	}

	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Power_Up_DAC_All
* Description		: Power down of  all DACs
* Input			: None
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Power_Down_DAC_All()
{
	status_t result = SEN_SUCCESS;

	result = AD5676R_write(AD5676_CMD_POWER,0x00, 0xFF); /* register is don't care */

	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Power_Up_DAC_All
* Description		: Power up of  all DACs
* Input			: None
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Power_Up_DAC_All()
{
	status_t result = SEN_SUCCESS;

	result = AD5676R_write(AD5676_CMD_POWER,0x00, 0x00); /* register is don't care */

	return result;
}

/*******************************************************************************
* Function Name		: AD5676_Power_Down_DAC_N
* Description		: Power down of individual DAC
* Input			: DAC number from 0 to 7
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AD5676_Power_Down_DAC_N (uint16_t dac_num, AD5676_PowerOption_t power_type)
{
	status_t result = SEN_SUCCESS;
	uint8_t bit_val = 0; // value to be set on the specific DAC

	if(power_type == onekO_To_GND)
	{
		bit_val = 1;
	}
	else if(power_type == three_State)
	{
		bit_val = 3;
	}

	result = AD5676R_write(AD5676_CMD_POWER,0x00, bit_val << dac_num*2); /* register is don't care. As DAC bits are in pairs shift the value by the corresponding DAC */

	return result;
}
