
#ifndef _UART__H
#define _UART__H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "GeneralDefines.h"


#define UART_MAX_BUFFER_SIZE		256

#define	UART_0 INDEX_UART_0
//#define	UART_1 INDEX_UART_1
//#define	UART_2 INDEX_UART_2
#define	UART_3 INDEX_UART_3
//#define	UART_4 INDEX_UART_4
//#define	UART_5 INDEX_UART_5
//#define	UART_6 INDEX_UART_6
//#define	UART_7 INDEX_UART_7

typedef enum
{
#ifdef UART_0
	INDEX_UART_0,
#endif
#ifdef UART_1
	INDEX_UART_1,
#endif
#ifdef UART_2
	INDEX_UART_2,
#endif
#ifdef UART_3
	INDEX_UART_3,
#endif
#ifdef UART_4
	INDEX_UART_4,
#endif
#ifdef UART_5
	INDEX_UART_5,
#endif
#ifdef UART_6
	INDEX_UART_6,
#endif
#ifdef UART_7
	INDEX_UART_7,
#endif
	
	MAX_UARTS
}UART_nameTypeDef;

#define UART1_TX_PORT	TX_UART1_USB_GPIO_Port
#define UART1_TX_PIN	TX_UART1_USB_Pin

#define UART1_RX_PORT RX_UART1_USB_GPIO_Port
#define UART1_RX_PIN RX_UART1_USB_Pin



enum
{
	//UART_USB = INDEX_UART_0,
	UART_485 = INDEX_UART_3,
	UART_232 = INDEX_UART_0,
	//UART_MODULE = INDEX_UART_6
};



extern void _Error_Handler(char *, int);

returnstatus_t UART_Init(UART_nameTypeDef uartChannel);
void UART_DesactivaUart(UART_nameTypeDef uartChannel);
uint32_t UART_QuantsBytesBufferRX(UART_nameTypeDef uartChannel);
returnstatus_t UART_Transmit(UART_nameTypeDef uartChannel, uint8_t* ptu8Dades, uint32_t NumDades);
returnstatus_t UART_Receive(UART_nameTypeDef uartChannel, uint8_t* ptu8Dades, uint32_t NumDades);
void UART_ResetTX(UART_nameTypeDef uartChannel);
void UART_ResetRX(UART_nameTypeDef uartChannel);
uint8_t UART_HiHaOverflow(UART_nameTypeDef uartChannel);
uint8_t UART_TransmitNOTFinished(UART_nameTypeDef uartChannel);


#endif
