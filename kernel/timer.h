
/**
* @file           : timer.h
* @autor          : liyou
* @date           : 2022/10/16
* All rights reserved.;
* @attention
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "list.h"
#include "def.h"
#include "irq.h"

void ly_timer_init(struct ly_timer* timer, 
                        void (*timeout)(void *parameter),
                        void       *parameter,
                        unsigned int   time,
                        unsigned char  flag);

void system_timer_init(void);

unsigned int __timer_stop(struct ly_timer* timer);

unsigned int ly_timer_stop(struct ly_timer* timer);

void  ly_timer_check(void);

extern unsigned int get_current_tick(void);

extern void ly_schedule(void);

unsigned int ly_timer_start(struct ly_timer* timer);

#endif



