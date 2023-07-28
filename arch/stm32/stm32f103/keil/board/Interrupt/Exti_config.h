/**
* @file           : Exti_config.h
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

/*
 * EXTI_Map_Config(GPIOx, BITx, TRIM); GPIOx: A - G, BITx: 0 - 15, TRIM: up->0, down->1, any->2
 * 
 *
 *
 */
#ifndef __Exti_config_H__
#define __Exti_config_H__

#include "gpio_register.h"

#define __is_GPIOx(GPIOx) \
	(GPIOx == 0 || GPIOx == 1 || GPIOx == 2 || 	\
	 GPIOx == 3 || GPIOx == 4 || GPIOx == 5)

#define __is_BITx(BITx)	\
	(BITx < 16UL)

#define __is_TRIM(TRIM)	\
	(TRIM == 0 || TRIM == 1 || TRIM == 2)

#define 	GPIO_A 		0
#define 	GPIO_B  	1
#define 	GPIO_C 		2
#define 	GPIO_D 		3
#define 	GPIO_E 		4
#define 	GPIO_F 		5
#define 	GPIO_G 		6 
#define 	down   		1  
#define 	up  		2  

#define __EXTI_Map_Config(GPIOx, BITx, TRIM)	\
({	\
	unsigned char EXTADDR;	\
	unsigned char EXTOFFSET; \
	EXTADDR     			= (BITx >> 2);	\
	EXTOFFSET				= ((BITx & 0x03u) << 2); \
	RCC->APB2ENR			|= 0x01;	\
	AFIO->EXTICR[EXTADDR]	&= ~(0x000F << EXTOFFSET);	\
	AFIO->EXTICR[EXTADDR]   |= GPIOx<<EXTOFFSET;	\
	EXTI->IMR				|= 1 << BITx;	\
 	if(TRIM & 0x01)	\
		EXTI->FTSR |= 1 << BITx;	\
	if(TRIM & 0x02)	\
		EXTI->RTSR |= 1 << BITx;	\
})

/*
 * GPIOx: A - G
 * BITx:  0 - 15
 * TRIM: up: 2 , down: 1, any: 3
*/ 
#define EXTI_Map_Config(GPIOx, BITx, TRIM)  \
({  \
    unsigned int __GPIOx = (GPIOx);  \
    unsigned int __BITx = (BITx);  \
    unsigned int __TRIM = (TRIM);  \
    (__is_GPIOx(__GPIOx) && \
	 __is_BITx(__BITx) && \
	 __is_TRIM(__TRIM)) ? \
	__EXTI_Map_Config(__GPIOx, __BITx, __TRIM) : \
	-1;	\
})




#endif
