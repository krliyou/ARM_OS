/**
* @file           : irq.h
* @autor          : liyou
* @date           : 2023/09/18
* All rights reserved.;
* @attention
*/

#ifndef __IRQ_H__
#define __IRQ_H__

void interrupt_enter(void);

void interrupt_leave(void);

extern unsigned int interrupt_enable(unsigned int val);

extern unsigned int interrupt_disable(void);

#define __interrupt_enter(void)     ly_interrupt_nest++
    
#define __interrupt_leave(void)     ly_interrupt_nest--

#endif


