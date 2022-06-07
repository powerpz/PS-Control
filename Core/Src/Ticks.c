
#include "Ticks.h"

uint32_t TICKS_GetTicks(void)
{
	uint32_t temp;
	//HAL_SuspendTick();
	temp = HAL_GetTick();
	//HAL_ResumeTick();
	return temp;
}

uint32_t TICKS_DiffTicks(uint32_t u32Mark)
{
	uint32_t temp;
	temp = TICKS_GetTicks();
	temp = (uint32_t)((uint32_t)temp - (uint32_t)u32Mark);
	temp = temp + 1;
	return temp;
}

