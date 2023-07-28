
/**
* @file           : scheduler.c
* @autor          : liyou
* @date           : 2022/09/16
* All rights reserved.;
* @attention
*/

#include "scheduler.h"

struct list_head ly_thread_priority_table[LY_THREAD_PRIORITY_MAX] = {0};

unsigned int ly_thread_ready_priority_group;
unsigned int ly_current_priority;

void system_scheduler_init(void)
{
	unsigned long offset;

	for (offset = 0; offset < LY_THREAD_PRIORITY_MAX; offset ++)
    {
        list_init(&ly_thread_priority_table[offset]);
    }

    ly_thread_ready_priority_group = 0;
}

void __schedule_insert_thread(struct ly_task *task)
{
	unsigned int level;

   	level = interrupt_disable();
	
    if (task == ly_current_thread)
    {
        task->stat =LY_THREAD_RUNNING | (task->stat & ~LY_THREAD_STAT_MASK);
        interrupt_enable(level);
    }

	if((task->stat & LY_THREAD_STAT_MASK) == LY_THREAD_RUNNING)
	{
		if((task->stat & LY_THREAD_STAT_MASK) != 0)
		{
			list_add_before(&(task->tlist), &(ly_thread_priority_table[task->current_priority]));
		}
		else
		{
			list_add_after(&(task->tlist), &(ly_thread_priority_table[task->current_priority]));
		}
	}
	else
	{
		list_add_before(&(task->tlist), &(ly_thread_priority_table[task->current_priority]));
	}

	ly_thread_ready_priority_group |= task->number_mask;
	
	task->stat = LY_THREAD_READY | (task->stat & ~LY_THREAD_STAT_MASK);

	interrupt_enable(level);
}

__asm int __rt_ffs(int value)
{
    CMP     r0, #0x00
    BEQ     exit

    RBIT    r0, r0
    CLZ     r0, r0
    ADDS    r0, r0, #0x01

exit
    BX      lr
}

static struct ly_task* __get_highest_priority_task(unsigned int* group)
{
    struct ly_task *highest_priority_task;
    unsigned int highest_ready_priority;

    highest_ready_priority = __rt_ffs(ly_thread_ready_priority_group) - 1;
    highest_priority_task = 
        list_entry(ly_thread_priority_table[highest_ready_priority].next, 
        struct ly_task, tlist);

     *group = highest_ready_priority;

    return highest_priority_task;
}

void system_scheduler_start()
{
    struct ly_task *highest_priority_task;
	unsigned int highest_ready_priority;

    highest_priority_task = __get_highest_priority_task(&highest_ready_priority);

    ly_current_thread = highest_priority_task;

    ly_remove_ready_task(highest_priority_task);

    highest_priority_task->stat = LY_THREAD_RUNNING;

    ly_context_switch_to((unsigned int)&highest_priority_task->sp);
}

void ly_schedule(void)
{
    unsigned int level;
    unsigned int highest_ready_priority;
    struct ly_task *to_thread;
    struct ly_task *from_thread;

    level = interrupt_disable();

    if (ly_thread_ready_priority_group != 0)
    {
        int need_insert_from_thread = 0;

        to_thread = __get_highest_priority_task(&highest_ready_priority);

        if ((ly_current_thread->stat & LY_THREAD_STAT_MASK) == LY_THREAD_RUNNING)
        {
            if (ly_current_thread->current_priority < highest_ready_priority)
            {
                to_thread = ly_current_thread;
            }
            else if (ly_current_thread->current_priority == highest_ready_priority && 
                    (ly_current_thread->stat & LY_THREAD_STAT_YIELD) == 0)
            {
                to_thread = ly_current_thread;
            }
            else
            {
                need_insert_from_thread = 1;
            }
            
           // ly_current_thread->stat &= ~LY_THREAD_STAT_YIELD;
        }

        if (to_thread != ly_current_thread)
        {
            ly_current_priority = highest_ready_priority;
            from_thread         = ly_current_thread;
            ly_current_thread   = to_thread;

            if (need_insert_from_thread)
            {
                from_thread->stat = LY_THREAD_READY | (from_thread->stat & ~LY_THREAD_STAT_MASK);
                __schedule_insert_thread(from_thread);
            }

            ly_remove_ready_task(to_thread);
            to_thread->stat = LY_THREAD_RUNNING | (to_thread->stat & ~LY_THREAD_STAT_MASK);

            
            if (ly_interrupt_nest == 0)
            {
                ly_context_switch((unsigned int)&from_thread->sp, (unsigned int)&to_thread->sp);

                interrupt_enable(level);
            }

            if (ly_interrupt_nest != 0)
            {
                ly_context_switch_interrupt((unsigned int)&from_thread->sp,
                            (unsigned int)&to_thread->sp, from_thread, to_thread);
            }                   
        }  
        else
        {
            ly_remove_ready_task(ly_current_thread);
            ly_current_thread->stat = 
            LY_THREAD_RUNNING | (ly_current_thread->stat & ~LY_THREAD_STAT_MASK);
        }
    }

    interrupt_enable(level);
}











