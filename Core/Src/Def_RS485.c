/*
 * File:   RS485.c
 * Author: ingenieria9
 *
 * Created on 22 de febrero de 2018, 15:19
 */



#include "Def_RS485.h"
#include "ticks.h"

/* DEFINES */
/* ------------------------------------------------------------------------------------------ */
// Temps que ha de passar sense trafic en el bus per considerar-lo lliure
#define RS485_TEMPS_INACTIVITAT     1
#define RS485_TEMPS_ENVIAMENT       1 // es una passada, pero de moment...


/* VARIABLES */
/* ------------------------------------------------------------------------------------------ */

uint8_t ptUART_CH[MAX_CHANNEL_RS485] = RS485_UART;

typedef struct 
{
    uint8_t u8EstatBusInter;
}TX485_Status;

TX485_Status xEstat485[MAX_CHANNEL_RS485];

/* FUNCIONES EXTERNAS */
/* ------------------------------------------------------------------------------------------ */
returnstatus_t RS485_Init(uint8_t u8Channel)
{

  RS485_TRANSMIT_DISABLE(u8Channel);
    
  xEstat485[u8Channel].u8EstatBusInter=0;

  return (UART_Init(ptUART_CH[u8Channel]));
}


returnstatus_t RS485_Transmit(uint8_t u8Channel, uint8_t* ptu8Dades, uint16_t u16NumDades, uint8_t u8LastPacket)
{
    returnstatus_t tmp;
    uint8_t i;

    tmp = RUNNING_HTP;
    
    switch(xEstat485[u8Channel].u8EstatBusInter)
    {
    	case 0:
            if(u16NumDades > 0)
            {
			  RS485_TRANSMIT_ENABLE(u8Channel); // TX_ENABLE a 1 -> comencem a enviar
              xEstat485[u8Channel].u8EstatBusInter++;
            }
            else
            {
							if(u8LastPacket == 1)
							{
									RS485_TRANSMIT_DISABLE(u8Channel);
							}
							tmp = SUCCESS_HTP;
            }
            break;
			case 1:
				switch(UART_Transmit(ptUART_CH[u8Channel], ptu8Dades, u16NumDades))
				{
					case SUCCESS_HTP: xEstat485[u8Channel].u8EstatBusInter++; break;
					case ERROR_HTP: xEstat485[u8Channel].u8EstatBusInter = 6; break;
					default: break;
				}
					break;
			case 2:
					i = UART_TransmitNOTFinished(ptUART_CH[u8Channel]);
					if(i == 0)
					{
							//s'han enviat totes les dades
							if(u8LastPacket == 1)
							{
									RS485_TRANSMIT_DISABLE(u8Channel);
							}
							xEstat485[u8Channel].u8EstatBusInter = 0;
							tmp = SUCCESS_HTP;
					}
					break;
			case 6: // Reintentem o donem error?
					RS485_TRANSMIT_DISABLE(u8Channel);
					xEstat485[u8Channel].u8EstatBusInter=0;
					tmp = ERROR_HTP;
					break;

			default:
					xEstat485[u8Channel].u8EstatBusInter=0;
					tmp = ERROR_HTP;
					break;
   }
   return tmp;
}


returnstatus_t RS485_Receive(uint8_t u8Channel, uint8_t* ptu8Dades, uint16_t u16NumDades)
{
	return (UART_Receive(ptUART_CH[u8Channel],ptu8Dades, u16NumDades));
}

uint32_t RS485_QuantsBytesBufferRX(uint8_t u8Channel)
{
    return (UART_QuantsBytesBufferRX(ptUART_CH[u8Channel]));
}


void RS485_ResetBuffers(uint8_t u8Channel)
{
    UART_ResetTX(ptUART_CH[u8Channel]);
    UART_ResetRX(ptUART_CH[u8Channel]);
}

