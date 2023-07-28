/**
* @file           : main.h
* @autor          : liyou
* @date           : 2022/01/02
* All rights reserved.
*/

#include "task.h"
#include "sys.h"
#include "scheduler.h"
#include "config.h"
#include "Systick_base.h"
#include "sem.h"
#include "mailbox.h"
#include "led.h"

extern void NvicInit(void);
extern void uart_init(unsigned int pclk2, unsigned int bound);
extern int putschar(char c);
extern int getschar(void);
