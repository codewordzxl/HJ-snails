#include "delay.h"
/******************—” ±****************/
void delayms(uint16_t ms)
{
	volatile uint32_t i = 0;
	while(ms--)
	{
		for (i = 0; i < 20000; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}	
}
