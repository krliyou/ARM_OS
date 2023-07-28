
/**
* @file           : task.c
* @autor          : liyou
* @date           : 2021/09/16
* All rights reserved.;
* @attention
*/

#include "task.h"

unsigned char ly_system_stack[512];
struct ly_task ly_system_thread;
struct ly_task *ly_current_thread = LY_NULL;

const char boot_log_buf[] = 
"___ __   __  ____   ______			\r\n" 
"| | \\\\   //// __ \\\\//____|    \r\n"
"| |  \\\\ // | |  | |||		       \r\n"
"| |   \\V/  | |  | |\\\\____          \r\n"
"| |   | |  | |  | |     \\\\     \r\n"
"| |___| |  | |__| |_____||       \r\n"
"|_____|_|  \\\\____//|____//       \r\n"; 
				


static void __thread_exit(void)
{
    struct ly_task *thread;
    signed long level;

    thread = get_current_task();

    level = interrupt_disable();

    ly_remove_ready_task(thread);

    __timer_stop(&thread->task_timer);

    thread->stat = LY_THREAD_CLOSE;

	interrupt_enable(level);

    ly_schedule();
}

static void __thread_timeout(void *parameter)
{
	struct ly_task *thread;
    signed long level;

    thread = (struct ly_task *)parameter;

    level = interrupt_disable();

    thread->error = -LY_ETIMEOUT;

    list_del(&(thread->tlist));

    __schedule_insert_thread(thread);

    interrupt_enable(level);

    ly_schedule();
}

unsigned int ly_task_init(struct ly_task *thread,
                        void (*entry)(void *parameter),
                        void             *parameter,
                        void             *stack_start,
                        unsigned int      stack_size,
                        unsigned char     priority,
                        unsigned int      tick)
{
    list_init(&(thread->tlist));

    thread->entry = (void*)entry;
    thread->parameter = parameter;
    thread->stack_addr = stack_start;
    thread->stack_size = stack_size;

	thread->sp =
	(void*)ly_stack_init(thread->entry, thread->parameter,
		                (void*)((char*)thread->stack_addr + 
                        thread->stack_size - sizeof(unsigned long)), 
                        (void *)__thread_exit);
	
    thread->init_priority = priority;
	thread->current_priority = priority;
	thread->error = LY_EOK;
	thread->stat = LY_THREAD_INIT;
	thread->init_tick = tick;
	thread->remaining_tick = tick;

	ly_timer_init(&(thread->task_timer), __thread_timeout , thread , 0, LY_TIMER_FLAG_ONE_SHOT);

	return LY_EOK;
}

static void ly_task_set_suspend_state(struct ly_task *thread, int suspend_flag)
{
    unsigned char stat = LY_THREAD_SUSPEND_UNINTERRUPTIBLE;

    switch (suspend_flag)
    {
    case LY_INTERRUPTIBLE:
        stat = LY_THREAD_SUSPEND_INTERRUPTIBLE;
        break;
    case LY_KILLABLE:
        stat = LY_THREAD_SUSPEND_KILLABLE;
        break;
    case LY_UNINTERRUPTIBLE:
        stat = LY_THREAD_SUSPEND_UNINTERRUPTIBLE;
        break;
    default:
        break;
    }
    thread->stat = stat | (thread->stat & ~LY_THREAD_STAT_MASK);
}

int ly_task_resume(struct ly_task *task)
{
    signed long level;

    if ((task->stat & LY_THREAD_SUSPEND) != LY_THREAD_SUSPEND)
		return -LY_ERROR;


    level = interrupt_disable();

	task->stat = LY_THREAD_READY | (task->stat & ~LY_THREAD_STAT_MASK);;

    list_del(&(task->tlist));

    __timer_stop(&task->task_timer);

	interrupt_enable(level);

    __schedule_insert_thread(task);

    return LY_EOK;
}

signed long ly_task_suspend_with_flag(struct ly_task *thread, int suspend_flag)
{
    signed long stat;
    signed long level;

    stat = thread->stat & LY_THREAD_STAT_MASK;
    
	if ((stat != LY_THREAD_READY) && (stat != LY_THREAD_RUNNING))
		return -LY_ERROR;

    if ((stat == LY_THREAD_RUNNING) && (thread != get_current_task()))
		return -LY_ERROR;
	
	level = interrupt_disable();

    ly_remove_ready_task(thread);
    ly_task_set_suspend_state(thread, suspend_flag);

    ly_timer_stop(&(thread->task_timer));

	interrupt_enable(level);

    return 0;
}

signed long ly_task_suspend(struct ly_task *thread)
{
    return ly_task_suspend_with_flag(thread, LY_UNINTERRUPTIBLE);
}

unsigned int ly_task_startup(struct ly_task *task)
{
	task->current_priority = task->init_priority;
	task->number_mask = 1 << (task->current_priority);
	task->stat = LY_THREAD_SUSPEND;

	ly_task_resume(task);
	
	return LY_EOK;
}

void ly_remove_ready_task(struct ly_task *task)
{
    signed long level;

    level = interrupt_disable();
	
    list_del(&(task->tlist));

	if (list_empty(&(ly_thread_priority_table[task->current_priority])))
    {
        ly_thread_ready_priority_group &= ~task->number_mask;
    }

    interrupt_enable(level);
}

void show_version(void)
{
	printf("\r\n====== START LY_OS NOW ======\r\n");
	printf("%s\r\n", boot_log_buf);    
	printf("Version: %s build %s\r\n\r\n", LY_OS_SW_VERSION, __DATE__);
}

void ly_thread_system_entry(void *parameter)
{
	finsh_shell_entry();
}

void ly_thread_idle_init(void)
{
    ly_task_init(&ly_system_thread,
            ly_thread_system_entry,
            NULL,
            ly_system_stack,
            sizeof(ly_system_stack),
            LY_THREAD_PRIORITY_MAX - 2,
            32);

	ly_task_startup(&ly_system_thread);
}

void os_startup(void)
{ 
	interrupt_disable();

	stm32_Sys_Init(9, 4);
	
	uart_init(72,115200);
	
	show_version();

	system_scheduler_init();
	
	system_timer_init();
	
	ly_thread_idle_init();
	
	Systick_init();

   
}	



