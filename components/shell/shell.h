/*
 * @file           : shell.h
 * @autor          : liyou
 * @date           : 2022/01/16
 */

#ifndef __SHELL_H__
#define __SHELL_H__

#include "config.h"
#include "usart_tranfer.h"
#include "shell.h"
#include "kservice.h"
#include "stdio.h"

#ifndef FINSH_CMD_SIZE
    #define FINSH_CMD_SIZE      20
#endif

#ifdef FINSH_USING_HISTORY
    #ifndef FINSH_HISTORY_LINES
        #define FINSH_HISTORY_LINES 15
    #endif
#endif

#ifdef FINSH_USING_AUTH
    #ifndef FINSH_PASSWORD_MAX
        #define FINSH_PASSWORD_MAX 10
    #endif
    #ifndef FINSH_PASSWORD_MIN
        #define FINSH_PASSWORD_MIN 6
    #endif
    #ifndef FINSH_PASSWORD
        #define FINSH_PASSWORD "123456"
    #endif
#endif

enum input_stat
{
    WAIT_NORMAL,
    WAIT_SPEC_KEY,
    WAIT_FUNC_KEY,
};

struct finsh_shell
{
	enum input_stat stat;

#ifdef FINSH_USING_HISTORY
    unsigned short     current_history;
    unsigned short     history_count;
    char cmd_history[FINSH_HISTORY_LINES][FINSH_CMD_SIZE];
#endif

#ifdef FINSH_USING_AUTH
    char password[FINSH_PASSWORD_MAX];
#endif

	char line[FINSH_CMD_SIZE + 1];
    unsigned short		line_position;
    unsigned short		line_curpos;

};

void finsh_shell_entry(void);
int finsh_system_init(void);

#endif
