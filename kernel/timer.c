
/**
* @file           : timer.c
* @autor          : liyou
* @date           : 2022/10/16
* All rights reserved.;
* @attention
*/

#include "timer.h"

static struct list_head __timer_list[LY_TIMER_N];

static void __timer_init(struct ly_timer *  timer,
			void (*timeout)(void *parameter),
			void *parameter, unsigned int time, 
			unsigned char flag)
{
    int i = 0;

    timer->flag  = flag;
    timer->flag &= ~LY_TIMER_FLAG_ACTIVATED;

    timer->timeout_func = timeout;
    timer->parameter    = parameter;

    timer->timeout_tick = 0;
    timer->init_tick    = time;

    for (i = 0; i < ly_TIMER_SKIP_LIST_LEVEL; i++)
    {
        list_init(&(timer->row[i]));
    }
}

void ly_timer_init(struct ly_timer *  timer,
			void (*timeout)(void *parameter),
			void *parameter, unsigned int time, 
			unsigned char flag)
{
	__timer_init( timer, timeout, parameter, time, flag);
}

void system_timer_init(void)
{
    unsigned long i;

    for (i = 0; i < sizeof(__timer_list) / sizeof(__timer_list[0]); i++)
    {
        list_init(__timer_list + i);
    }
}

static inline void __timer_remove(struct ly_timer* timer)
{
    int i;

    for (i = 0; i < ly_TIMER_SKIP_LIST_LEVEL; i++)
    {
        list_del(&timer->row[i]);
    }
}

unsigned int __timer_stop(struct ly_timer* timer)
{
    __timer_remove(timer);
    
    timer->flag &= ~LY_TIMER_FLAG_ACTIVATED;

    return LY_EOK;
}

unsigned int ly_timer_stop(struct ly_timer* timer)
{
    signed long level;

    level = interrupt_disable();

    __timer_stop(timer);

    interrupt_enable(level);

    return LY_EOK;
}

unsigned int ly_timer_start(struct ly_timer* timer)
{
	unsigned int level;
	struct list_head *timer_list;
	struct list_head *row_head[ly_TIMER_SKIP_LIST_LEVEL];
	unsigned int row_lvl = 0;
	unsigned int need_schedule;

	level = interrupt_disable();

    need_schedule = 0;

    __timer_remove(timer);

    timer->flag &= ~LY_TIMER_FLAG_ACTIVATED;
    timer->timeout_tick = get_current_tick() + timer->init_tick;

	timer_list = __timer_list;
    row_head[0]  = &timer_list[0];

    for (row_lvl = 0; row_lvl < ly_TIMER_SKIP_LIST_LEVEL; row_lvl++)
    {
		for (; row_head[row_lvl] != timer_list[row_lvl].prev;
             row_head[row_lvl]  = row_head[row_lvl]->next)
        {
            struct ly_timer *t;
            struct list_head *p = row_head[row_lvl]->next;

            t = list_entry(p, struct ly_timer, row[row_lvl]);

            if ((t->timeout_tick - timer->timeout_tick) == 0)
                continue;
            
            if ((t->timeout_tick - timer->timeout_tick) < LY_TICK_MAX / 2)
                break;
        }

        if (row_lvl != ly_TIMER_SKIP_LIST_LEVEL - 1)
            row_head[row_lvl + 1] = row_head[row_lvl] + 1;
    }
    
    list_add_after(&(timer->row[ly_TIMER_SKIP_LIST_LEVEL - 1]), row_head[ly_TIMER_SKIP_LIST_LEVEL - 1]);

    timer->flag |= LY_TIMER_FLAG_ACTIVATED;

	if (need_schedule)
    {
        ly_schedule();
    }

    interrupt_enable(level);

    return LY_EOK;
}

void ly_timer_check(void)
{
    unsigned int level;
    struct ly_timer *t;
    unsigned int current_tick;

    current_tick = get_current_tick();

    level = interrupt_disable();

    while (!list_empty(&__timer_list[LY_TIMER_N - 1]))
    {
        t = list_entry(__timer_list[LY_TIMER_N - 1].next,   \
            struct ly_timer, row[LY_TIMER_N - 1]);
        
        if ((current_tick - t->timeout_tick) < LY_TICK_MAX / 2)
        {
            __timer_remove(t);
            
            if (!(t->flag & LY_TIMER_FLAG_PERIODIC))
                t->flag &= ~LY_TIMER_FLAG_ACTIVATED;        

            t->timeout_func(t->parameter);

            current_tick = get_current_tick();
         
            if ((t->flag & LY_TIMER_FLAG_PERIODIC) &&
                (t->flag & LY_TIMER_FLAG_ACTIVATED))
            {
                t->flag &= ~LY_TIMER_FLAG_ACTIVATED;
                ly_timer_start(t);
            }
        }
        else break;
    }

    interrupt_enable(level);
}












