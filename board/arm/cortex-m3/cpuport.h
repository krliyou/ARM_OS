
/**
* @file           : cpuport.h
* @autor          : liyou
* @date           : 2022/09/16
* All rights reserved.;
* @attention
*/

#ifndef __CPUPORT_H__
#define __CPUPORT_H__

#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

struct exception_stack_frame
{
    unsigned int r0;
    unsigned int r1;
    unsigned int r2;
    unsigned int r3;
    unsigned int r12;
    unsigned int lr;
    unsigned int pc;
    unsigned int psr;
};

struct stack_frame
{
    unsigned int r4;
    unsigned int r5;
    unsigned int r6;
    unsigned int r7;
    unsigned int r8;
    unsigned int r9;
    unsigned int r10;
    unsigned int r11;

    struct exception_stack_frame exception_stack_frame;
};

unsigned char *ly_stack_init(void *tentry, 
						void *parameter,
						unsigned char *stack_addr,
						void *texit);

#endif
