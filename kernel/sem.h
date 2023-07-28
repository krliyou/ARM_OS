
/*
 * @file           : sem.h
 * @autor          : liyou
 * @date           : 2022/11/16
 * All rights reserved.;
 * @attention
 */

#ifndef __SEM_H
#define __SEM_H

#include "list.h"
#include "irq.h"
#include "task.h"
#include "def.h"

struct ly_semaphore
{
	struct list_head	suspend_thread;     
	unsigned short 	value;               
	unsigned short		eserved;  
	unsigned char 		flag;
};


signed long ly_sem_init(struct ly_semaphore *sem, 
					const char *name, unsigned int value, unsigned char flag);

signed long ly_sem_take(struct ly_semaphore *sem, unsigned int time);
 
signed long ly_sem_release(struct ly_semaphore *sem);

#endif
