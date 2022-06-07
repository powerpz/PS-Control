
#include "UART.h"

typedef struct
{
	uint8_t ptu8BufferTX[UART_MAX_BUFFER_SIZE];
	
	uint32_t u32InitRXBufferPointer;
	uint32_t u32EndRXBufferPointer;
	uint32_t u32BytesInRXBuffer;
	uint8_t ptu8BufferRX[UART_MAX_BUFFER_SIZE];
	union
	{
		uint32_t value;
		struct
		{
			uint32_t u32TransmitNotFinished	:	1;
			uint32_t u32OverflowRX					: 1;
			uint32_t u32Dummy								: 30;
		}bit_field;
	}status;
}UARTStruct_Control;

UARTStruct_Control uart_control[MAX_UARTS];

/**
  * @brief Inicializacion modulo.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @note   Inicia el modulo uart en caso de no estarlo e inicia proceso recepcion.
  * @retval ERROR_HTP		-> uart no definida
	*					SUCCESS_HTP
	*					RUNNING_HTP	-> modulo iniciandose
  */
returnstatus_t UART_Init(UART_nameTypeDef uartChannel)
{
	HAL_StatusTypeDef hal_uart_state;
	returnstatus_t return_uart_init;
	if(uartChannel >= MAX_UARTS)
		return ERROR_HTP;
	
	uart_control[uartChannel].status.value = 0;
	
	uart_control[uartChannel].u32InitRXBufferPointer = 0;
	uart_control[uartChannel].u32EndRXBufferPointer = 0;
	uart_control[uartChannel].u32BytesInRXBuffer = 0;
#ifdef UART_0
	if(uartChannel == INDEX_UART_0)
	{
		if(huart1.gState == HAL_UART_STATE_RESET)
		{
			/* si no se ha iniciado lo incio */
			MX_USART1_UART_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart1, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
#ifdef UART_1
	if(uartChannel == INDEX_UART_1)
	{	
		if(huart2.gState == HAL_UART_STATE_RESET)
		{
			/* si no se ha iniciado lo incio */
			MX_USART2_UART_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart2, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
#ifdef UART_2
	if(uartChannel == INDEX_UART_2)
	{
		if(huart3.gState == HAL_UART_STATE_RESET)
		{
			/* si no se ha iniciado lo incio */
			MX_USART3_UART_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart3, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
#ifdef UART_3
	if(uartChannel == INDEX_UART_3)
	{
		if(huart4.gState == HAL_UART_STATE_RESET)
		{
			/* si no se ha iniciado lo incio */
			MX_UART4_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart4, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
#ifdef UART_4
	if(uartChannel == INDEX_UART_4)
	{
		if(huart5.gState == HAL_UART_STATE_RESET)
		{
			/* if it hasn't started */
			MX_USART5_UART_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart5, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
#ifdef UART_5
	if(uartChannel == INDEX_UART_5)
	{
		if(huart6.gState == HAL_UART_STATE_RESET)
		{
			/* si no se ha iniciado lo incio */
			MX_USART6_UART_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart6, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
#ifdef UART_6
	if(uartChannel == INDEX_UART_6)
	{
		if(huart7.gState == HAL_UART_STATE_RESET)
		{
			/* si no se ha iniciado lo incio */
			MX_USART7_UART_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart7, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
#ifdef UART_7
	if(uartChannel == INDEX_UART_7)
	{
		if(huart8.gState == HAL_UART_STATE_RESET)
		{
			/* si no se ha iniciado lo incio */
			MX_USART8_UART_Init();
		}
		hal_uart_state = HAL_UART_Receive_IT(&huart8, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_init = ERROR_HTP; break;
			case HAL_OK: return_uart_init = SUCCESS_HTP; break;
			case HAL_BUSY: return_uart_init = RUNNING_HTP; break;
		}
		
		return return_uart_init;
	}
#endif
	return ERROR_HTP;
}


/**
  * @brief Desactiva modulo uart y fija los pines a su valor inicial.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @note   aborta el proceso de recepcion y cualquier transmision en curso
  * @retval none
  */
void UART_DesactivaUart(UART_nameTypeDef uartChannel)
{
	UART_HandleTypeDef *uartHandle = NULL;
	/* abortar cualquier transmision y recepcion en curso, desactivar perif y poner pines al valor incial */
#ifdef UART_0
	if(uartChannel == INDEX_UART_0)
	{
		uartHandle = &huart1;
	}
#endif
#ifdef UART_1
	if(uartChannel == INDEX_UART_1)
	{
		uartHandle = &huart2;
	}
#endif
#ifdef UART_2
	if(uartChannel == INDEX_UART_2)
	{
		uartHandle = &huart3;
	}
#endif
#ifdef UART_3
	if(uartChannel == INDEX_UART_3)
	{
		uartHandle = &huart4;
	}
#endif
#ifdef UART_4
	if(uartChannel == INDEX_UART_4)
	{
		uartHandle = &huart5;
	}
#endif
#ifdef UART_5
	if(uartChannel == INDEX_UART_5)
	{
		uartHandle = &huart6;
	}
#endif
#ifdef UART_6
	if(uartChannel == INDEX_UART_6)
	{
		uartHandle = &huart7;
	}
#endif
#ifdef UART_7
	if(uartChannel == INDEX_UART_7)
	{
		uartHandle = &huart8;
	}
#endif
	if(uartHandle->gState != HAL_UART_STATE_RESET)
	{
		HAL_UART_Abort_IT(uartHandle);
		HAL_UART_DeInit(uartHandle);
	}
	uart_control[uartChannel].status.value = 0;
	
	uart_control[uartChannel].u32InitRXBufferPointer = 0;
	uart_control[uartChannel].u32EndRXBufferPointer = 0;
	uart_control[uartChannel].u32BytesInRXBuffer = 0;
}


/**
  * @brief Devuelve num bytes en el buffer de recepcion.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @note   deshabilita la interrupcion de recepcion para acceder a u32BytesInRXBuffer.
  * @retval HAL status
  */
uint32_t UART_QuantsBytesBufferRX(UART_nameTypeDef uartChannel)
{
	uint32_t temp;
	UART_HandleTypeDef* uartHandle = NULL;
	if(uartChannel >= MAX_UARTS)
		return 0;

#ifdef UART_0	
	if(uartChannel == INDEX_UART_0)
	{
		uartHandle = &huart1;
	}
#endif
#ifdef UART_1	
	if(uartChannel == INDEX_UART_1)
	{
		uartHandle = &huart2;
	}
#endif
#ifdef UART_2	
	if(uartChannel == INDEX_UART_2)
	{
		uartHandle = &huart3;
	}
#endif
#ifdef UART_3	
	if(uartChannel == INDEX_UART_3)
	{
		uartHandle = &huart4;
	}
#endif
#ifdef UART_4	
	if(uartChannel == INDEX_UART_4)
	{
		uartHandle = &huart5;
	}
#endif
#ifdef UART_5	
	if(uartChannel == INDEX_UART_5)
	{
		uartHandle = &huart6;
	}
#endif
#ifdef UART_6	
	if(uartChannel == INDEX_UART_6)
	{
		uartHandle = &huart7;
	}
#endif
#ifdef UART_7	
	if(uartChannel == INDEX_UART_7)
	{
		uartHandle = &huart8;
	}
#endif
	
	if(uartHandle == NULL)
	{
		return 0;
	}
	__HAL_UART_DISABLE_IT(uartHandle, UART_IT_RXNE);
	temp = uart_control[uartChannel].u32BytesInRXBuffer;
	__HAL_UART_ENABLE_IT(uartHandle, UART_IT_RXNE);
	
	return temp;
}


/**
  * @brief Encola datos para enviar.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @param ptu8Dades puntero con los datos a enviar.
  * @param NumDades numero de datos a enviar.
  * @note   Copia en el buffer interno los datos a enviar e inicia proceso.
  * @retval returnstatus_t
  */
returnstatus_t UART_Transmit(UART_nameTypeDef uartChannel, uint8_t* ptu8Dades, uint32_t NumDades)
{
	returnstatus_t return_uart_tx;
	HAL_StatusTypeDef hal_uart_state = HAL_ERROR;
	UART_HandleTypeDef* uartHandle = NULL;
	uint32_t i;
	
	if(uartChannel >= MAX_UARTS)
		return ERROR_HTP;
	#ifdef UART_0	
	if(uartChannel == INDEX_UART_0)
	{
		uartHandle = &huart1;
	}
#endif
#ifdef UART_1	
	if(uartChannel == INDEX_UART_1)
	{
		uartHandle = &huart2;
	}
#endif
#ifdef UART_2	
	if(uartChannel == INDEX_UART_2)
	{
		uartHandle = &huart3;
	}
#endif
#ifdef UART_3	
	if(uartChannel == INDEX_UART_3)
	{
		uartHandle = &huart4;
	}
#endif
#ifdef UART_4	
	if(uartChannel == INDEX_UART_4)
	{
		uartHandle = &huart5;
	}
#endif
#ifdef UART_5	
	if(uartChannel == INDEX_UART_5)
	{
		uartHandle = &huart6;
	}
#endif
#ifdef UART_6	
	if(uartChannel == INDEX_UART_6)
	{
		uartHandle = &huart7;
	}
#endif
#ifdef UART_7	
	if(uartChannel == INDEX_UART_7)
	{
		uartHandle = &huart8;
	}
#endif
	
	if(uartHandle == NULL)
	{
		return ERROR_HTP;
	}
	
	if(uart_control[uartChannel].status.bit_field.u32TransmitNotFinished == 0)
	{
		for(i = 0; i < NumDades; i++)
		{
			uart_control[uartChannel].ptu8BufferTX[i] = ptu8Dades[i];
		}
		
		hal_uart_state = HAL_UART_Transmit_IT(uartHandle, uart_control[uartChannel].ptu8BufferTX, NumDades);
		
		switch(hal_uart_state)
		{
			default:
			case HAL_ERROR: return_uart_tx = ERROR_HTP; break;	/* tamanyo 0 o no hay buffer definido o buffer no alineado */
			case HAL_BUSY: return_uart_tx = RUNNING_HTP; break;	/* solo puede dar si no esta disponible para transmitir, pero ya he comprobado no estar transmitiendo */
			case HAL_OK: 
				uart_control[uartChannel].status.bit_field.u32TransmitNotFinished = 1; 
				return_uart_tx = SUCCESS_HTP;
				break;
		}
	}
	else
	{
		return_uart_tx = RUNNING_HTP;
	}
	
	return return_uart_tx;
}



/**
  * @brief Lee del buffer interno.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @param ptu8Dades puntero a los datos de salida.
  * @param NumDades numero de datos a recibir.
  * @note   deshabilita interrupcion de recepcion para decrementar u32BytesInRXBuffer.
  * @retval returnstatus_t
  */
returnstatus_t UART_Receive(UART_nameTypeDef uartChannel, uint8_t* ptu8Dades, uint32_t NumDades)
{
	returnstatus_t temp;
	UART_HandleTypeDef* uartHandle = NULL;
	uint32_t i;
	
#ifdef UART_0	
	if(uartChannel == INDEX_UART_0)
	{
		uartHandle = &huart1;
	}
#endif
#ifdef UART_1	
	if(uartChannel == INDEX_UART_1)
	{
		uartHandle = &huart2;
	}
#endif
#ifdef UART_2	
	if(uartChannel == INDEX_UART_2)
	{
		uartHandle = &huart3;
	}
#endif
#ifdef UART_3	
	if(uartChannel == INDEX_UART_3)
	{
		uartHandle = &huart4;
	}
#endif
#ifdef UART_4	
	if(uartChannel == INDEX_UART_4)
	{
		uartHandle = &huart5;
	}
#endif
#ifdef UART_5	
	if(uartChannel == INDEX_UART_5)
	{
		uartHandle = &huart6;
	}
#endif
#ifdef UART_6	
	if(uartChannel == INDEX_UART_6)
	{
		uartHandle = &huart7;
	}
#endif
#ifdef UART_7	
	if(uartChannel == INDEX_UART_7)
	{
		uartHandle = &huart8;
	}
#endif
	
	if(uartHandle == NULL)
	{
		return ERROR_HTP;
	}
	
	if(NumDades > uart_control[uartChannel].u32BytesInRXBuffer)
	{
		temp = ERROR_HTP;
	}
	else{
		for(i = 0; i < NumDades; i++)
		{
			ptu8Dades[i] = uart_control[uartChannel].ptu8BufferRX[uart_control[uartChannel].u32InitRXBufferPointer++];
			uart_control[uartChannel].u32InitRXBufferPointer = uart_control[uartChannel].u32InitRXBufferPointer&(UART_MAX_BUFFER_SIZE-1);
		}

		__HAL_UART_DISABLE_IT(uartHandle, UART_IT_RXNE);
		uart_control[uartChannel].u32BytesInRXBuffer -= NumDades;
		__HAL_UART_ENABLE_IT(uartHandle, UART_IT_RXNE);
		
		temp = SUCCESS_HTP;
	}
	return temp;
}


/**
  * @brief resetea buffer de transmision.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @note   aborta transimision que estuviera en curso
  * @retval none
  */
void UART_ResetTX(UART_nameTypeDef uartChannel)
{

#ifdef UART_0
	if(uartChannel == INDEX_UART_0)
	{
		HAL_UART_AbortTransmit(&huart1);
	}
#endif
#ifdef UART_1
	if(uartChannel == INDEX_UART_1)
	{
		HAL_UART_AbortTransmit(&huart2);
	}
#endif
#ifdef UART_2
	if(uartChannel == INDEX_UART_2)
	{
		HAL_UART_AbortTransmit(&huart3);
	}
#endif
#ifdef UART_3
	if(uartChannel == INDEX_UART_3)
	{
		HAL_UART_AbortTransmit(&huart4);
	}
#endif
#ifdef UART_4
	if(uartChannel == INDEX_UART_4)
	{
		HAL_UART_AbortTransmit(&huart5);
	}
#endif
#ifdef UART_5
	if(uartChannel == INDEX_UART_5)
	{
		HAL_UART_AbortTransmit(&huart6);
	}
#endif
#ifdef UART_6
	if(uartChannel == INDEX_UART_6)
	{
		HAL_UART_AbortTransmit(&huart7);
	}
#endif
#ifdef UART_7
	if(uartChannel == INDEX_UART_7)
	{
		HAL_UART_AbortTransmit(&huart8);
	}
#endif
	
	uart_control[uartChannel].status.bit_field.u32TransmitNotFinished = 0;
}


/**
  * @brief resetea buffer de recepcion
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @note   aborta recepcion en curso y vuelve a lanzar proceso de recepcion
  * @retval none
  */
void UART_ResetRX(UART_nameTypeDef uartChannel)
{
	UART_HandleTypeDef* uartHandle = NULL;
	
#ifdef UART_0	
	if(uartChannel == INDEX_UART_0)
	{
		uartHandle = &huart1;
	}
#endif
#ifdef UART_1	
	if(uartChannel == INDEX_UART_1)
	{
		uartHandle = &huart2;
	}
#endif
#ifdef UART_2	
	if(uartChannel == INDEX_UART_2)
	{
		uartHandle = &huart3;
	}
#endif
#ifdef UART_3	
	if(uartChannel == INDEX_UART_3)
	{
		uartHandle = &huart4;
	}
#endif
#ifdef UART_4	
	if(uartChannel == INDEX_UART_4)
	{
		uartHandle = &huart5;
	}
#endif
#ifdef UART_5	
	if(uartChannel == INDEX_UART_5)
	{
		uartHandle = &huart6;
	}
#endif
#ifdef UART_6	
	if(uartChannel == INDEX_UART_6)
	{
		uartHandle = &huart7;
	}
#endif
#ifdef UART_7	
	if(uartChannel == INDEX_UART_7)
	{
		uartHandle = &huart8;
	}
#endif
	
	if(uartHandle == NULL)
	{
		return;
	}
	uart_control[uartChannel].u32BytesInRXBuffer = 0;
	uart_control[uartChannel].u32InitRXBufferPointer = 0;
	uart_control[uartChannel].u32EndRXBufferPointer = 0;
	uart_control[uartChannel].status.bit_field.u32OverflowRX = 0;
	
	HAL_UART_AbortReceive(uartHandle);
	HAL_UART_Receive_IT(uartHandle, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32InitRXBufferPointer), 1);
	
}

/**
  * @brief devuelve shi ha habido overflow en la recepcion.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @note   En caso de haber overflow se debera reactivar el proceso de recepcion mediante llamada a UART_ResetRX.
  * @retval 0, no overflow
	*					1, overflow
  */
uint8_t UART_HiHaOverflow(UART_nameTypeDef uartChannel)
{
	return uart_control[uartChannel].status.bit_field.u32OverflowRX;
}

/**
  * @brief devuelve si se ha completado el envio de datos.
  * @param uartChannel INDEX_UART_x, donde x puede ser un valor entre 0 y 7 y que este definido en UART.h
  * @note   
  * @retval 0, envio completado
	*					1, envio en curso
  */
uint8_t UART_TransmitNOTFinished(UART_nameTypeDef uartChannel)
{
	return uart_control[uartChannel].status.bit_field.u32TransmitNotFinished;
}


/**
  * @brief Tx Transfer completed callback.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UART_nameTypeDef uartChannel = MAX_UARTS;
#ifdef UART_0
	if(huart->Instance == USART1)
	{
			uartChannel = INDEX_UART_0;
	}
#endif
#ifdef UART_1
	if(huart->Instance == USART2)
	{
		uartChannel = INDEX_UART_1;
	}
#endif
#ifdef UART_2
	if(huart->Instance == USART3)
	{
		uartChannel = INDEX_UART_2;
	}
#endif
#ifdef UART_3
	if(huart->Instance == UART4)
	{
		uartChannel = INDEX_UART_3;
	}
#endif
#ifdef UART_4
	if(huart->Instance == USART5)
	{
		uartChannel = INDEX_UART_4;
	}
#endif
#ifdef UART_5
	if(huart->Instance == USART6)
	{
		uartChannel = INDEX_UART_5;
	}
#endif
#ifdef UART_6
	if(huart->Instance == USART7)
	{
		uartChannel = INDEX_UART_6;
	}
#endif
#ifdef UART_7
	if(huart->Instance == USART8)
	{
		uartChannel = INDEX_UART_7;
	}
#endif
	if(uartChannel >= MAX_UARTS)
	{
		return;
	}
	uart_control[uartChannel].status.bit_field.u32TransmitNotFinished = 0;
}


/**
  * @brief Rx Transfer completed callback.
  * @param huart UART handle.
  * @note   Anyade en el buffer interno el byte recibido. Activa flag de overflow en caso de buffer lleno
	*					si no hay overflow vuelve a activar proceso de recepcion
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* se ha recibido 1 byte */
	UART_nameTypeDef uartChannel = MAX_UARTS;
#ifdef UART_0
	if(huart->Instance == USART1)
	{
			uartChannel = INDEX_UART_0;
	}
#endif
#ifdef UART_1
	if(huart->Instance == USART2)
	{
		uartChannel = INDEX_UART_1;
	}
#endif
#ifdef UART_2
	if(huart->Instance == USART3)
	{
		uartChannel = INDEX_UART_2;
	}
#endif
#ifdef UART_3
	if(huart->Instance == UART4)
	{
		uartChannel = INDEX_UART_3;
	}
#endif
#ifdef UART_4
	if(huart->Instance == USART5)
	{
		uartChannel = INDEX_UART_4;
	}
#endif
#ifdef UART_5
	if(huart->Instance == USART6)
	{
		uartChannel = INDEX_UART_5;
	}
#endif
#ifdef UART_6
	if(huart->Instance == USART7)
	{
		uartChannel = INDEX_UART_6;
	}
#endif
#ifdef UART_7
	if(huart->Instance == USART8)
	{
		uartChannel = INDEX_UART_7;
	}
#endif
	if(uartChannel >= MAX_UARTS)
	{
		return;
	}
	
	if(uart_control[uartChannel].u32BytesInRXBuffer < UART_MAX_BUFFER_SIZE)
	{
		uart_control[uartChannel].u32EndRXBufferPointer = (uart_control[uartChannel].u32EndRXBufferPointer+1)&(UART_MAX_BUFFER_SIZE-1);
		uart_control[uartChannel].u32BytesInRXBuffer++;
		HAL_UART_Receive_IT(huart, (uint8_t *)(uart_control[uartChannel].ptu8BufferRX+uart_control[uartChannel].u32EndRXBufferPointer), 1);
	}
	else
	{
		uart_control[uartChannel].status.bit_field.u32OverflowRX = 1; // Hi ha error d'overflow.
	}
}

