
/**
* @file           : config.h
* @autor          : liyou
* @date           : 2021/09/02
* All rights reserved.
*/

#define LY_OS_SW_VERSION 	"1.0.0"

/* Kernel */
#define LY_THREAD_PRIORITY_MAX          32
#define LY_TIMER_N                      1
#define LY_TICK_MAX                 0xffffffff
#define ly_TIMER_SKIP_LIST_LEVEL 1
#define SystemCoreClock 8000000
#define LY_TICK_PER_SECOND 1000
#define LY_SEM_VALUE_MAX 65535U
#define LY_MB_ENTRY_MAX 65535U

/* Finsh Configuration */
#define LY_USING_FINSH
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES	        5
//#define FINSH_USING_AUTH
#define FINSH_PASSWORD 				"1"
#define FINSH_PASSWORD_MIN 			 1
#define FINSH_PASSWORD_MAX 			5
#define FINSH_PROMPT "LY_OYA />"
#define FINSH_CMD_SIZE				20
#define FINSH_AUTO_COMPLETE

/* USART Configuration */
#define USART_REC_LEN  			128 

