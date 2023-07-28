
/**
* @file           : def.h
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#ifndef __TYPES_H__
#define __TYPES_H__

#include "config.h"

struct list_head 
{
	struct list_head *next, *prev;
};

struct ly_timer
{
    struct list_head row[ly_TIMER_SKIP_LIST_LEVEL];

    void (*timeout_func)(void *parameter);             
    void            *parameter;                         

    unsigned int    init_tick;                         
    unsigned int    timeout_tick;                  

    unsigned char   flag;
};

struct ly_task
{
    void            *sp;                                
    void            *entry;                                 
    void            *parameter;                             
    void            *stack_addr;                             
    unsigned int    stack_size;
    unsigned char   stat;
    unsigned char   current_priority;
    unsigned char   init_priority;                       
    unsigned long   init_tick;
    unsigned int    number_mask; 
    signed long	 error; 
    unsigned long   remaining_tick; 
    struct          ly_timer task_timer;
    struct          list_head  tlist; 
    void (*cleanup)(struct ly_task *tid); 
};

/* boolean type definitions */
#define LY_TRUE                         1               /**< boolean true  */
#define LY_FALSE                        0               /**< boolean fails */

/* null pointer definition */
#define LY_NULL                         0

#define LY_EOK                          0               /**< There is no error */
#define LY_ERROR                        1               /**< A generic error happens */
#define LY_ETIMEOUT                     2               /**< Timed out */
#define LY_EFULL                        3               /**< The resource is full */
#define LY_EEMPTY                       4               /**< The resource is empty */
#define LY_ENOMEM                       5               /**< No memory */
#define LY_ENOSYS                       6               /**< No system */
#define LY_EBUSY                        7               /**< Busy */
#define LY_EIO                          8               /**< IO error */
#define LY_EINTR                        9               /**< Interrupted system call */
#define LY_EINVAL                       10              /**< Invalid argument */
#define LY_ETRAP                        11              /**< Trap event */
#define LY_ENOENT                       12              /**< No entry */
#define LY_ENOSPC                       13              /**< No space left */

enum
{
    LY_INTERRUPTIBLE = 0,
    LY_KILLABLE,
    LY_UNINTERRUPTIBLE,
};

#define RALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

typedef __SIZE_TYPE__ size_t;

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member)     \
({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); \
})

#define LY_THREAD_INIT                       0x00              
#define LY_THREAD_CLOSE                      0x01               
#define LY_THREAD_READY                      0x02                
#define LY_THREAD_RUNNING                    0x03              
#define LY_THREAD_SUSPEND                    0x04               

#define LY_THREAD_STAT_YIELD                 0x08         
#define LY_THREAD_STAT_MASK                  0x07


#define LY_TIMER_FLAG_DEACTIVATED       0x0             
#define LY_TIMER_FLAG_ACTIVATED         0x1             
#define LY_TIMER_FLAG_ONE_SHOT          0x0             
#define LY_TIMER_FLAG_PERIODIC          0x2             

#define LY_TIMER_FLAG_HARD_TIMER        0x0             
#define LY_TIMER_FLAG_SOFT_TIMER        0x4           

#define LY_TIMER_CTRL_SET_TIME          0x0            
#define LY_TIMER_CTRL_GET_TIME          0x1             
#define LY_TIMER_CTRL_SET_ONESHOT       0x2            
#define LY_TIMER_CTRL_SET_PERIODIC      0x3            
#define LY_TIMER_CTRL_GET_STATE         0x4            
#define LY_TIMER_CTRL_GET_REMAIN_TIME   0x5             
#define LY_TIMER_CTRL_GET_FUNC          0x6             
#define LY_TIMER_CTRL_SET_FUNC          0x7            
#define LY_TIMER_CTRL_GET_PARM          0x8           
#define LY_TIMER_CTRL_SET_PARM          0x9            

#define LY_IPC_FLAG_FIFO                0x00           
#define LY_IPC_FLAG_PRIO                0x01           

#define LY_IPC_CMD_UNKNOWN              0x00           
#define LY_IPC_CMD_RESET                0x01           
#define LY_IPC_CMD_GET_STATE            0x02            

#define LY_WAITING_FOREVER              -1             
#define LY_WAITING_NO                   0             

#define LY_SIGNAL_COMMON_WAKEUP_MASK         0x02
#define LY_SIGNAL_KILL_WAKEUP_MASK           0x01

#define LY_THREAD_SUSPEND_INTERRUPTIBLE      0x04                                                      

#define LY_THREAD_SUSPEND_KILLABLE           \
	(LY_THREAD_SUSPEND | LY_SIGNAL_COMMON_WAKEUP_MASK)                            

#define LY_THREAD_SUSPEND_UNINTERRUPTIBLE    \
	(LY_THREAD_SUSPEND | LY_SIGNAL_COMMON_WAKEUP_MASK | LY_SIGNAL_KILL_WAKEUP_MASK) 


#endif

