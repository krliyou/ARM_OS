/**
* @file           : Systick_base.h
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#ifndef __SYSTICK_BASE_H__
#define __SYSTICK_BASE_H__

#include "gpio_register.h"
#include "Nvic_config.h"
#include "irq.h"
#include "clock.h"
#include "config.h"

#define Systick_Set_ms(n_ms)    \
({  \
    unsigned long __n_ms = (n_ms); \
    (__n_ms > 0  && __n_ms < 1865) ?    \
    (SysTick->LOAD = __n_ms*T_ms) : \
    -1; \
})

#define Systick_Set_us(n_us)    \
({  \
    unsigned long __n_us = (n_us); \
    (__n_us > 0 && __n_us < 1865000) ?  \
    SysTick->LOAD   = __n_us*T_us : \
    -1; \
})

void Systick_init(void);

#endif

