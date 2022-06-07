/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */


#ifndef SYSTEM_H
#define	SYSTEM_H

#include "GeneralDefines.h"
#include "stm32f4xx_hal.h"

extern memory_map_t MemoryMap;

void SYS_Init(void);

returnstatus_t SYS_GetParam_Char(uint16_t u16Parametro, uint8_t *ptu8Value, uint16_t u16NumReg);

returnstatus_t SYS_SetParam_Char(uint16_t u16Parametro, uint8_t *ptu8Value, uint16_t u16NumReg);

returnstatus_t SYS_Action(uint8_t u8Operation, uint8_t u8Order, uint32_t u32Addr, uint8_t* ptu8Dades, uint16_t u16NumDades);

#endif	/* SYSTEM_H */
