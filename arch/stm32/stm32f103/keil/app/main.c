
/**
* @file           : main.c
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#include "main.h"

unsigned char task1_stack[512];
unsigned char task2_stack[512];

struct ly_task 		task1;
struct ly_task 		task2; 

void task1_entry(void *parameter)
{
	
}

void task2_entry(void *parameter)
{
}

int main()
{
	os_startup();

	LED_Init();

	LED0_open();
	LED1_open();
    
	ly_task_init(&task1, task1_entry, LY_NULL, 
		&task1_stack[0], sizeof(task1_stack), 3, 25);
	ly_task_startup(&task1);


	ly_task_init(&task2, task2_entry, LY_NULL, 
		&task2_stack[0], sizeof(task2_stack), 3, 50);
	ly_task_startup(&task2);


	system_scheduler_start();
	
	while(1);
}





