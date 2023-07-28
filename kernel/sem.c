
/*
 * @file           : sem.c
 * @autor          : liyou
 * @date           : 2022/11/16
 * All rights reserved.;
 * @attention
 */

#include "sem.h"

struct ly_semaphore ly_sem = {0};

signed long ly_sem_init(struct ly_semaphore *sem, 
					const char *name, unsigned int value, unsigned char flag)
{
	sem->value = (unsigned int)value;

	sem->flag = flag;

	list_init(&(sem->suspend_thread));

    return 0;
}

signed long ly_sem_detach(struct ly_semaphore *sem)
{
	struct ly_task *thread;
    unsigned int level;
	struct list_head *suspend = &(sem->suspend_thread);

    while (!list_empty(suspend))
    {
		level = interrupt_disable();
        thread = list_entry(suspend->next, struct ly_task, tlist);
        thread->error = -LY_ERROR;
        ly_task_resume(thread);
		interrupt_enable(level);
    }

    return 0;
}

static signed long _rt_sem_take(struct ly_semaphore *sem, unsigned int timeout, int suspend_flag)
{
    signed long level;
    struct ly_task *thread;
    signed long ret;
	
    level = interrupt_disable();

    if (sem->value > 0)
    {
        sem->value --;
        interrupt_enable(level);
    }
    else
    {
		if (timeout == 0)
        {
            interrupt_enable(level);
            return -LY_ETIMEOUT;
        }
        else
        {
			thread = get_current_task();
            thread->error = -LY_EINTR;
			if ((thread->stat & LY_THREAD_SUSPEND) != LY_THREAD_SUSPEND)
			{
				signed long ret = ly_task_suspend_with_flag(thread, suspend_flag);
				list_add_before(&(thread->tlist), &(sem->suspend_thread));
			}

            if (ret != LY_EOK)
            {
                interrupt_enable(level);
                return ret;
            }

            if (timeout > 0)
            {		
				thread->task_timer.init_tick = timeout;

                ly_timer_start(&(thread->task_timer));
            }

			interrupt_enable(level);

            ly_schedule();

            if (thread->error != LY_EOK)
            {
                return thread->error;
            }
        }
    }

    return 0;
}

signed long ly_sem_take(struct ly_semaphore *sem, unsigned int time)
{
    return _rt_sem_take(sem, time, LY_UNINTERRUPTIBLE);
}

signed long ly_sem_release(struct ly_semaphore *sem)
{
    signed long level;    
	int need_schedule;
	struct ly_task *thread;

    need_schedule = LY_FALSE;

    level = interrupt_disable();

    if (!list_empty(&sem->suspend_thread))
    {
		thread = list_entry(sem->suspend_thread.next, struct ly_task, tlist);

		thread->error = LY_EOK;

		ly_task_resume(thread);
        need_schedule = LY_TRUE;
    }
    else
    {
        if(sem->value < LY_SEM_VALUE_MAX)
        {
            sem->value ++;
        }
        else
        {
            interrupt_enable(level); 	
            return -LY_EFULL; 	
        }
    }
	
	interrupt_enable(level);

    if (need_schedule == LY_TRUE)
        ly_schedule();

    return LY_EOK;
}


