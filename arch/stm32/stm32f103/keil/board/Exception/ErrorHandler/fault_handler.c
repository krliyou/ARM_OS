/**
* @file           : fault_handler.c
* @autor          : liyou
* @date           : 2022/11/19
* All rights reserved.;
* @attention
*/

#include "fault_handler.h"

void NMI_Handler(void)
{
	
}

void MemManage_Handler(void)
{
	while (1)
	{
		
	}
}

void BusFault_Handler(void)
{
	while (1)
	{
		
	}
}

void ly_hw_hard_fault_exception(struct exception_info * exception_info)
{
	unsigned int *app_sp;
	
	int i;
	app_sp = (unsigned int *)(exception_info + 1);  /* context + 16*4 */
    
    printf("psr: 0x%08x\r\n", exception_info->psr);
    printf("r00: 0x%08x\r\n", exception_info->r0);
    printf("r01: 0x%08x\r\n", exception_info->r1);
    printf("r02: 0x%08x\r\n", exception_info->r2);
    printf("r03: 0x%08x\r\n", exception_info->r3);
    printf("r04: 0x%08x\r\n", exception_info->r4);
    printf("r05: 0x%08x\r\n", exception_info->r5);
    printf("r06: 0x%08x\r\n", exception_info->r6);
    printf("r07: 0x%08x\r\n", exception_info->r7);
    printf("r08: 0x%08x\r\n", exception_info->r8);
    printf("r09: 0x%08x\r\n", exception_info->r9);
    printf("r10: 0x%08x\r\n", exception_info->r10);
    printf("r11: 0x%08x\r\n", exception_info->r11);
	printf("r12: 0x%08x\r\n", exception_info->r12);
	printf(" lr: 0x%08x\r\n", exception_info->lr);
    printf(" pc: 0x%08x\r\n", exception_info->pc);
    printf(" exc_return: 0x%08x\r\n", exception_info->exc_return);

	printf("\r\n");
	
	printf("stacks is : \r\n");
	i = 0;
	for (i = 0; i < 1024; i++)
	{
		printf("%08x ", *app_sp);
		app_sp++;
		if (i % 16 == 0)
			printf("\r\n");		
	}
	printf("\r\n");

	while (1);
}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}


void PPP_IRQHandler(void)
{

}
