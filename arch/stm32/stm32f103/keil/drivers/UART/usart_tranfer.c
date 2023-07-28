
/**
* @file           : usart_tranfer.c
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#include "usart_tranfer.h"


struct kfifo pkfifo = {0};
unsigned char USART_RX_BUF[USART_REC_LEN]; 

/*USART1: Tx -- PB9, Rx --- PB10 */
void uart_init(unsigned int pclk2, unsigned int bound)
{  	 
	float temp;
    unsigned short mantissa;
	unsigned short fraction;
	
    RCC->APB2ENR        |= 1 << 2;   
	RCC->APB2ENR        |= 1 << 14;  
	GPIOA->CRH			&= 0XFFFFF00F;
	GPIOA->CRH			|= 0X000008B0;
	RCC->APB2RSTR       |= 1 << 14;   
	RCC->APB2RSTR       &= ~(1<<14);  

	temp = (float)(pclk2*1000000)/(bound << 4);
	mantissa = temp;				 
	fraction = (temp-mantissa)*16; 
    mantissa <<= 4;
	mantissa += fraction;  	   

 	USART1->BRR          = mantissa;  
	USART1->CR1         |= 0X200C;
		
	USART1->CR1|=1<<5;   

	NVIC_EnableIRQ(USART1_IRQn);

	NVIC_SetPriority(USART1_IRQn, 14, 0);
	
	kfifo_init(&pkfifo, USART_RX_BUF, USART_REC_LEN);
}

void USART1_IRQHandler(void)
{		
	if(USART1->SR & (1<<5))
	{
		kfifo_in(&pkfifo, &(USART1->DR), 1);
	}
}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);				
	USART1->DR = (unsigned char) ch;      	
	return ch;
}

unsigned char get_str(void)
{
	unsigned char s;
	while (0 == kfifo_out(&pkfifo, &s, 1));
	return s;
}

		

	