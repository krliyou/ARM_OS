
/**
* @file           : clock.h
* @autor          : liyou
* @date           : 2022/09/30
* All rights reserved.;
* @attention
*/

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "task.h"
#include "irq.h"
#include "timer.h"

void ly_tick_increase(void);
unsigned int get_current_tick(void);

#endif

