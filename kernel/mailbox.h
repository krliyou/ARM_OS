
/*
 * @file           : mailxbox.h
 * @autor          : liyou
 * @date           : 2022/11/16
 * All rights reserved.;
 * @attention
 */

#ifndef __MAILXBOX_H
#define __MAILXBOX_H

#include "list.h"
#include "task.h"
#include "config.h"

struct ly_mailbox
{
	unsigned long	*msg_pool;                     
	unsigned short	size;                       
    unsigned short	entry;                         
    unsigned short	in_offset;                    
    unsigned short out_offset;   
	unsigned char	flag;
	struct list_head	suspend_recver_thread;   
    struct list_head	suspend_sender_thread;
};

signed long ly_mb_init(struct ly_mailbox *mb, void *msgpool, 
				unsigned long size, unsigned char flag);

signed long ly_mb_detach(struct ly_mailbox *mb);

signed long ly_mb_send_wait(struct ly_mailbox *mb, 
				unsigned long value, unsigned long timeout);

signed long ly_mb_send(struct ly_mailbox *mb, unsigned long value);

signed long ly_mb_recv(struct ly_mailbox *mb, \
			unsigned long *value, unsigned long timeout);

#endif
