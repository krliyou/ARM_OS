
/**
* @file           : usart_tranfer.h
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#ifndef __USART_TRANFER_H__
#define __USART_TRANFER_H__

#include "gpio_register.h"
#include "kfifo.h"
#include "Nvic_config.h"
#include "config.h"

void uart_init(unsigned int pclk2, unsigned int bound);

unsigned char get_str(void);


#endif
