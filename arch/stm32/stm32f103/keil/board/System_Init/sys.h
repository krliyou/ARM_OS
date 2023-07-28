/**
* @file           : sys.h
* @autor          : liyou
* @date           : 2022/01/02
* All rights reserved.
* @attention
*/

#ifndef __SYS_H__
#define __SYS_H__

#include <stdint.h>
#include "gpio_register.h"
#include "Nvic_config.h"

#define BITBAND(addr, bitnum) \
	((addr & 0xF0000000) + 0x2000000 + \
	((addr & 0xFFFFF) << 5) + (bitnum << 2))

#define MEM_ADDR(addr)  \
	*((volatile unsigned long  *)(addr)) 

#define BIT_ADDR(addr, bitnum)   \
	MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr    (GPIOA_BASE + 12) 	//0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE + 12) 	//0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE + 12) 	//0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE + 12) 	//0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE + 12) 	//0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE + 12) 	//0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE + 12) 	//0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE + 8) 		//0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE + 8) 		//0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE + 8) 		//0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE + 8) 		//0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE + 8) 		//0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE + 8) 		//0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE + 8) 		//0x40011E08 
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define IS_PLL(pLL) 	\
	(((pLL) > 1 && (pLL) < 17))

#define IS_GROUP(NVIC_Group) 	\
	((NVIC_Group) == 0 || (NVIC_Group) == 1 || \
	(NVIC_Group) == 2 || (NVIC_Group) == 3 || \
	(NVIC_Group) == 4)

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */ 
 /*!< Vector Table base offset field. 
  * This value must be a multiple of 0x200
  */
#define VECT_TAB_OFFSET			0x0

#define __Reset_RCC()   \
({  \
    RCC->APB1RSTR           = 0x00000000;     \
	RCC->APB2RSTR           = 0x00000000;     \
    RCC->AHBENR             = 0x00000014;     \
  	RCC->APB2ENR            = 0x00000000;     \
  	RCC->APB1ENR            = 0x00000000;     \
	RCC->CR                 |= 0x00000001;    \
	RCC->CFGR               &= 0xF8FF0000;    \
	RCC->CR                 &= 0xFEF6FFFF;    \
	RCC->CR                 &= 0xFFFBFFFF;    \
	RCC->CFGR               &= 0xFF80FFFF;    \
	RCC->CIR                =  0x00000000;    \
})

#define __RCC_CFGR_IS(CFGR) \
({\
    (CFGR != 0x02)  ?   \
    ({  \
        do \
        { \
            CFGR = CFGR >> 2;   \
		    CFGR &= 0x03;   \
        }while(CFGR != 0x02);   \
    }) : \
    -1;  \
})

#define __Stm32_Sys_Init(pLL)   \
({  \
	__Reset_RCC();  \
 	RCC->CR     |= 0x00010000;  \
	while(!(RCC->CR >> 17)); \
	RCC->CFGR   = 0X00000400; \
	pLL         -= 2;   \
	RCC->CFGR   |= pLL << 18; \
	RCC->CFGR   |= 1 << 16;   \
	FLASH->ACR  |= 0x32;    \
	RCC->CR     |= 0x01000000;  \
	while(!(RCC->CR >> 25));    \
	RCC->CFGR   |= 0x00000002;  \
    unsigned char __CFGR = RCC->CFGR; \
    __RCC_CFGR_IS(__CFGR);    \
})

#define stm32_Sys_Init(pLL, NVIC_Group)   \
({  \
    unsigned int __pLL = (pLL);    \
	unsigned int __NVIC_Group = (NVIC_Group);    \
     ((IS_PLL(__pLL)) &&	\
	  (IS_GROUP(__NVIC_Group)))? \
	 ({	\
		__Stm32_Sys_Init(__pLL); \
		__NVIC_SetPriorityGrouping(__NVIC_Group);	\
	 }) :	\
    -1; \
})

#define WFI_SET(void) (__ASM volatile("wfi")) 

#define INTX_DISABLE(void) (__ASM volatile("cpsid i"))

#define INTX_ENABLE(void) (__ASM volatile("cpsie i"))  


#endif
