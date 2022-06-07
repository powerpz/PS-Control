
#ifndef _TICKS__H
#define _TICKS__H

#include "stdint.h"
#include "stm32f4xx_hal.h"

uint32_t TICKS_GetTicks(void);

uint32_t TICKS_DiffTicks(uint32_t ulMark);


#endif
