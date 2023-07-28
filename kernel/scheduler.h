/**
* @file           : scheduler.h
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "list.h"
#include "def.h"
#include "timer.h"
#include "irq.h"

extern volatile unsigned int ly_interrupt_nest;

void system_scheduler_init(void);

void __schedule_insert_thread(struct ly_task *task);

extern struct ly_task *ly_current_thread;

void system_scheduler_start(void);

extern void ly_context_switch_to(unsigned int sp);

extern void ly_remove_ready_task(struct ly_task *task);

void ly_schedule(void);

extern void ly_context_switch(unsigned int from, unsigned int to);

extern void ly_context_switch_interrupt
    (unsigned int from, unsigned int to, struct ly_task *from_thread, struct ly_task *to_thread);

#endif

