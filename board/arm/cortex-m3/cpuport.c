
/**
* @file           : cpuport.c
* @autor          : liyou
* @date           : 2022/09/16
* All rights reserved.;
* @attention
*/

#include "cpuport.h"

unsigned long ly_interrupt_from_thread;
unsigned long ly_interrupt_to_thread;
unsigned long ly_thread_switch_interrupt_flag;

unsigned char *ly_stack_init(void *tentry, 
				void *parameter, 
				unsigned char *stack_addr,
				void *texit)
{
    struct stack_frame *stack_frame;
    unsigned char      *stk;
    unsigned long       i;

    stk  = stack_addr + sizeof(unsigned int);
    stk  = (unsigned char *)ALIGN_DOWN((unsigned int)stk, 8);
    stk -= sizeof(struct stack_frame);

    stack_frame = (struct stack_frame *)stk;

    for (i = 0; i < sizeof(struct stack_frame) / sizeof(unsigned int); i ++)
    {
        ((unsigned int *)stack_frame)[i] = 0xdeadbeef;
    }

    stack_frame->exception_stack_frame.r0  = (unsigned long)parameter;
    stack_frame->exception_stack_frame.r1  = 0;                        
    stack_frame->exception_stack_frame.r2  = 0;                      
    stack_frame->exception_stack_frame.r3  = 0;                        
    stack_frame->exception_stack_frame.r12 = 0;                     
    stack_frame->exception_stack_frame.lr  = (unsigned long)texit;     
    stack_frame->exception_stack_frame.pc  = (unsigned long)tentry;
    stack_frame->exception_stack_frame.psr = 0x01000000L;

    return stk;
}


