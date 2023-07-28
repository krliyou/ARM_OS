
/**
* @file           : clock.c
* @autor          : liyou
* @date           : 2022/09/30
* All rights reserved.;
* @attention
*/

#include "clock.h"

static unsigned int ly_tick = 0;

void ly_tick_increase(void)
{
    struct ly_task *current_task;
    unsigned int level;

    level = interrupt_disable();
    
    ++ ly_tick;

    current_task = get_current_task();

    -- current_task->remaining_tick;

    if (current_task->remaining_tick == 0)
    {
        current_task->remaining_tick = current_task->init_tick;
        current_task->stat |= LY_THREAD_STAT_YIELD;
        interrupt_enable(level);
        ly_schedule();
    }

    interrupt_enable(level);

    ly_timer_check();
}

unsigned int get_current_tick(void)
{
    return ly_tick;
}









