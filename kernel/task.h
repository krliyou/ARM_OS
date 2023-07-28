/**
* @file           : task.c
* @autor          : liyou
* @date           : 2022/10/16
* All rights reserved.;
* @attention
*/

#ifndef __TASK_H__
#define __TASK_H__

#include "timer.h"
#include "list.h"
#include "scheduler.h"
#include "irq.h"
#include "def.h"
#include "sys.h"
#include "usart_tranfer.h"
#include "Systick_base.h"
#include "shell.h"

#define get_current_task() ly_current_thread

extern struct list_head ly_thread_priority_table[LY_THREAD_PRIORITY_MAX];
extern unsigned int ly_thread_ready_priority_group;

unsigned int ly_task_init(struct ly_task *thread,
                        void (*entry)(void *parameter),
                        void             *parameter,
                        void             *stack_start,
                        unsigned int      stack_size,
                        unsigned char     priority,
                        unsigned int      tick);


extern unsigned char *ly_stack_init(void *tentry, 
                                    void *parameter,
                                    unsigned char *stack_addr,
                                    void *texit);

unsigned int ly_task_startup(struct ly_task *task);

int ly_task_resume(struct ly_task *task);

signed long ly_task_suspend(struct ly_task *thread);

signed long ly_task_suspend_with_flag(struct ly_task *thread, int suspend_flag);

void ly_remove_ready_task(struct ly_task *task);

void os_startup(void);

void show_version(void);

#endif


