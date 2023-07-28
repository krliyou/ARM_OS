
/**
* @file           : irq.h
* @autor          : liyou
* @date           : 2022/09/20
* All rights reserved.;
* @attention
*/

#include "irq.h"

volatile unsigned int ly_interrupt_nest;

void interrupt_enter(void)
{
	interrupt_disable();
    ly_interrupt_nest ++; 
}

void interrupt_leave(void)
{
    unsigned int level;
    level = interrupt_disable();
    ly_interrupt_nest --;
    interrupt_enable(level);
}



