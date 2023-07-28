
/**
* @file           : Delay_Base.h
* @autor          : liyou
* @date           : 2022/01/17
* All rights reserved.
*/

/*
 * delay_init(); 
 *
 * delay_us(n_us); (n_us > 0 && n_us < 1865000)
 *
 * delay_ms(n_ms);  (n_ms > 0 && n_ms < 1865)
 *
 *
*/

#ifndef __DELAY_BASE_H__
#define __DELAY_BASE_H__

#include "types.h"
#include "gpio_register.h"

#define T_us	9						
#define T_ms 	9000				

#define delay_init()    Systick_init();

#define __delay_while()  \
({ \
    unsigned int temp; \
	SysTick->VAL    = 0x00;  \
	SysTick->CTRL   = 0x01;  \
    do\
	{\
		temp = SysTick->CTRL;   \
	}while  \
    ((temp & 0x01) && !(temp & (1 << 16)));     \
	SysTick->CTRL   = 0x00; \
	SysTick->VAL    = 0X00; \
})

#define delay_us(n_us)   \
({  \
    unsigned int __n_us = (unsigned int)(n_us); \
    (__n_us > 0 && __n_us < 1865000) ?  \
    ({ \
        SysTick->LOAD   = (__n_us)*T_us;  \
        __delay_while();    \
    }): \
})

#define delay_ms(n_ms)  \
({  \
    unsigned int __n_ms = (unsigned int)(n_ms); \
    (__n_ms > 0  && __n_ms < 1865) ?    \
    ({  \
        SysTick->LOAD   = (__n_ms)*T_ms;  \
        __delay_while();    \
    }): \
    -1; \
})

#endif

