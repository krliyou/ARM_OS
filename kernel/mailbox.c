

/*
 * @file           : mailxbox.c
 * @autor          : liyou
 * @date           : 2022/11/16
 * All rights reserved.;
 * @attention
 */

#include "mailbox.h"

struct ly_mailbox *ly_mailbox_t;

static inline signed long _ipc_list_resume_all
					(struct list_head *list)
{
	struct ly_task *thread;
	signed long level;

    while (!list_empty(list))
    {
		level = interrupt_disable();
        thread = list_entry(list->next, struct ly_task, tlist);
        thread->error = -LY_ERROR;
        ly_task_resume(thread);
		interrupt_enable(level);
    }

    return 0;
}

signed long ly_mb_init(struct ly_mailbox *mb, void *msgpool, 
				unsigned long size, unsigned char flag)
{
    mb->msg_pool   = (unsigned long*)msgpool;
    mb->size       = (unsigned short)size;
    mb->entry      = 0;
    mb->in_offset  = 0;
    mb->out_offset = 0;
	mb->flag = flag;

    list_init(&(mb->suspend_sender_thread));
	list_init(&(mb->suspend_recver_thread));
    
	return 0;
}

signed long ly_mb_detach(struct ly_mailbox *mb)
{
	_ipc_list_resume_all(&(mb->suspend_recver_thread));
    _ipc_list_resume_all(&(mb->suspend_sender_thread));
    return LY_EOK;
}

static inline unsigned int _ipc_list_suspend(struct list_head *list,
                                       struct ly_task *thread,
                                       unsigned char flag,
                                       int suspend_flag)
{
    if ((thread->stat & LY_THREAD_SUSPEND) != LY_THREAD_SUSPEND)
    {
        signed long ret = ly_task_suspend_with_flag(thread, suspend_flag);
        if (ret != LY_EOK)
        {
            return ret;
        }
    }

    switch (flag)
    {
		case LY_IPC_FLAG_FIFO:
			list_add_before(&(thread->tlist), list);
			break;

		case LY_IPC_FLAG_PRIO:
			{
				struct list_head *n;
				struct ly_task *sthread;

				for (n = list->next; n != list; n = n->next)
				{
					sthread = list_entry(n, struct ly_task, tlist);

					if (thread->current_priority < sthread->current_priority)
					{
						list_add_before(&(thread->tlist), &(sthread->tlist));
						break;
					}
				}
	
				if (n == list)
					list_add_before(&(thread->tlist), list);
			}

			break;

		default:
			break;
		}

    return 0;
}

static signed long _ly_mb_send_wait(struct ly_mailbox *mb,
                         unsigned long	value,
                         signed int timeout,
                         int suspend_flag)
{
    struct ly_task *thread;
    signed int level;
    unsigned int tick_delta;
    signed long ret;

    tick_delta = 0;
    thread = get_current_task();

    level = interrupt_disable();

    if (mb->entry == mb->size && timeout == 0)
    {
        interrupt_enable(level);
        return -LY_EFULL;
    }

    while (mb->entry == mb->size)
    {
		thread->error = -LY_EINTR;

        if (timeout == 0)
        {
			interrupt_enable(level);
            return -LY_EFULL;
        }

        ret = _ipc_list_suspend(&(mb->suspend_sender_thread),
                            thread,
                            mb->flag,
                            suspend_flag);
        if (ret != LY_EOK)
        {
			interrupt_enable(level);
            return ret;
        }

        if (timeout > 0)
        {
            tick_delta = get_current_tick();
			thread->task_timer.init_tick = timeout;
            ly_timer_start(&(thread->task_timer));
        }

		interrupt_enable(level);

        ly_schedule();

        if (thread->error != LY_EOK)
        {
            return thread->error;
        }

        level = interrupt_disable();

        if (timeout > 0)
        {
            tick_delta = get_current_tick() - tick_delta;
            timeout -= tick_delta;
            if (timeout < 0)
                timeout = 0;
        }
	}

    mb->msg_pool[mb->in_offset] = value;
    ++ mb->in_offset;
    if (mb->in_offset >= mb->size)
        mb->in_offset = 0;

	if(mb->entry < LY_MB_ENTRY_MAX)
    {
        mb->entry ++;
    }
    else
    {
        interrupt_enable(level); 
        return -LY_EFULL; 
    }

    if (!list_empty(&mb->suspend_recver_thread))
    {
		thread = list_entry(mb->suspend_recver_thread.next, struct ly_task, tlist);

		thread->error = LY_EOK;

		ly_task_resume(thread);

		interrupt_enable(level);

        ly_schedule();

        return LY_EOK;
    }

	interrupt_enable(level);

    return LY_EOK;
}

signed long ly_mb_send_wait(struct ly_mailbox *mb, unsigned long value, unsigned long timeout)
{
    return _ly_mb_send_wait(mb, value, timeout, LY_UNINTERRUPTIBLE);
}

signed long ly_mb_send(struct ly_mailbox *mb, unsigned long value)
{
    return ly_mb_send_wait(mb, value, 0);
}

static signed long _ly_mb_recv(struct ly_mailbox *mb, 
						unsigned long *value, signed int timeout, int suspend_flag)
{
    struct ly_task *thread;
    signed long level;
    unsigned int tick_delta;
    signed long ret;

    tick_delta = 0;

    thread = get_current_task();

    level = interrupt_disable();

    if (mb->entry == 0 && timeout == 0)
    {
		interrupt_enable(level);
        return -LY_ETIMEOUT;
    }

    while (mb->entry == 0)
    {
        thread->error = -LY_EINTR;

        if (timeout == 0)
        {
			interrupt_enable(level);
            thread->error = -LY_ETIMEOUT;
            return -LY_ETIMEOUT;
        }

        ret = _ipc_list_suspend(&(mb->suspend_recver_thread),
                            thread,
                            mb->flag,
                            suspend_flag);
        if (ret != LY_EOK)
        {
			interrupt_enable(level);
            return ret;
        }

        if (timeout > 0)
        {
            tick_delta = get_current_tick();

			thread->task_timer.init_tick = timeout;

            ly_timer_start(&(thread->task_timer));
        }

		interrupt_enable(level);

        ly_schedule();

        if (thread->error != LY_EOK)
        {
            return thread->error;
        }

        level = interrupt_disable();

        if (timeout > 0)
        {
            tick_delta = get_current_tick() - tick_delta;
            timeout -= tick_delta;
            if (timeout < 0)
                timeout = 0;
        }
    }

    *value = mb->msg_pool[mb->out_offset];

    ++ mb->out_offset;
    if (mb->out_offset >= mb->size)
        mb->out_offset = 0;

    if(mb->entry > 0)
    {
        mb->entry --;
    }

    if (!list_empty(&(mb->suspend_sender_thread)))
    {
		thread = list_entry(mb->suspend_sender_thread.next, struct ly_task, tlist);
		thread->error = LY_EOK;
		ly_task_resume(thread);
		interrupt_enable(level);
        ly_schedule();
        return LY_EOK;
    }

	interrupt_enable(level);

    return LY_EOK;
}

signed long ly_mb_recv(struct ly_mailbox *mb, unsigned long *value, unsigned long timeout)
{
    return _ly_mb_recv(mb, value, timeout, LY_UNINTERRUPTIBLE);
}


