/**
* @file           : fault_handler.h
* @autor          : liyou
* @date           : 2022/11/19
* All rights reserved.;
* @attention
*/

#ifndef __FAULT_HANDLER_H__
#define __FAULT_HANDLER_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stdio.h"
#include "config.h"

struct exception_info
{
    unsigned int exc_return;
    unsigned int r4;
    unsigned int r5;
    unsigned int r6;
    unsigned int r7;
    unsigned int r8;
    unsigned int r9;
    unsigned int r10;
    unsigned int r11;
    unsigned int r0;
    unsigned int r1;
    unsigned int r2;
    unsigned int r3;
    unsigned int r12;
    unsigned int lr;
    unsigned int pc;
    unsigned int psr;
};

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);

void ly_hw_hard_fault_exception(struct exception_info * exception_info);

#ifdef __cplusplus
}
#endif

#endif


