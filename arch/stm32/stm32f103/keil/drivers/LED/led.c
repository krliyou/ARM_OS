
/**
* @file           : led.c
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#include "led.h"

/* Red light: PA1 
 * green light: PB8
 * down actice#define LED0    PAout(1)	
 */	

/* Red light: PA1 
 * green light: PB8
 * down actice
 */

void LED_Init(void)
{
    RCC->APB2ENR    |= 1<<3;    
    RCC->APB2ENR    |= 1<<2;     
	GPIOB->CRH      &= 0XFFFFFFF0; 
	GPIOB->CRH      |= 0X00000003; 
	GPIOB->ODR      |= 1<<8;  
	GPIOA->CRL      &= 0XFFFFFF0F; 
	GPIOA->CRL      |= 0X00000030; 
    GPIOA->ODR      |= 1<<1;
}







