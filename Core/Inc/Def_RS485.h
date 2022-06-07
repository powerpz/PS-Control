/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef RS485_H
#define	RS485_H
#include "GeneralDefines.h"
#include "UART.h"
#include "stm32f4xx_hal.h"

enum
{
	RS485_SLV,
	
	MAX_CHANNEL_RS485
};

#define RS485_UART {UART_3}

//#define RS485_TRANSMIT_ENABLE_DIRECTION(u8Channel);
//#define RS485_TRANSMIT_ENABLE_DIRECTION     TRISAbits.TRISA6 = 0;
//#define RS485_TRANSMIT_ENABLE_DIRECTION(u8Channel)     TRISGbits.TRISG3 = 0;

//#define RS485_TRANSMIT_ENABLE(u8Channel)    if(u8Channel==RS485_SLV) HAL_GPIO_WritePin(TX_ENABLE_485_GRAL_GPIO_Port, TX_ENABLE_485_GRAL_Pin, GPIO_PIN_SET); 	\
//																						else HAL_GPIO_WritePin(TX_ENABLE_485_SENS_GPIO_Port, TX_ENABLE_485_SENS_Pin, GPIO_PIN_SET);
//#define RS485_TRANSMIT_DISABLE(u8Channel)		if(u8Channel==RS485_SLV) HAL_GPIO_WritePin(TX_ENABLE_485_GRAL_GPIO_Port, TX_ENABLE_485_GRAL_Pin, GPIO_PIN_RESET); \
//																						else HAL_GPIO_WritePin(TX_ENABLE_485_SENS_GPIO_Port, TX_ENABLE_485_SENS_Pin, GPIO_PIN_RESET);

//#define RS485_TRANSMIT_ENABLE(u8Channel)    LATGbits.LATG3 = 1;
//#define RS485_TRANSMIT_DISABLE(u8Channel)   LATGbits.LATG3 = 0;
#define RS485_TRANSMIT_ENABLE(u8Channel)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
#define RS485_TRANSMIT_DISABLE(u8Channel)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);


returnstatus_t RS485_Init(uint8_t u8Channel);

returnstatus_t RS485_Receive(uint8_t u8Channel, uint8_t* ptu8Dades, uint16_t u16NumDades);

returnstatus_t RS485_Transmit(uint8_t u8Channel, uint8_t* ptu8Dades, uint16_t u16NumDades, uint8_t u8LastPacket);

uint32_t RS485_QuantsBytesBufferRX(uint8_t u8Channel);

void RS485_ResetBuffers(uint8_t u8Channel);



#endif	/* RS485_H */
