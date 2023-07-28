
/**
* @file           : led.h
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#ifndef __LED_H
#define __LED_H

#include "gpio_register.h"

#define LED0_open()  PAout(1) = 0
#define LED1_open()  PBout(8) = 0

#define LED0_close()  PAout(1) = 1
#define LED1_close()  PBout(8) = 1

void LED_Init(void);


#endif
