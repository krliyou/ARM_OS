/**
* @file           : Systick_base.c
* @autor          : liyou
* @date           : 2022/09/16
* All rights reserved.;
* @attention
*/

#include "Systick_base.h"

void Systick_init(void)
{
	SysTick->CTRL = (1 << 1) | (1 << 0);
	SysTick->CTRL &= ~(1 << 2);
   
	SysTick->LOAD  = (unsigned int)(SystemCoreClock / LY_TICK_PER_SECOND - 1UL); 

	//NVIC_SetPriority(SysTick_IRQn, 15, 0);

}								    


void SysTick_Handler(void)
{
	unsigned int val;

	val = interrupt_disable();

    __interrupt_enter();

    ly_tick_increase();

    __interrupt_leave();

	interrupt_enable(val);
}


