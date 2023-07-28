/*
 * @file           : msh.h
 * @autor          : RTT
 * @date           : 2022/01/16
 */

#ifndef __M_SHELL__
#define __M_SHELL__

#include "stdio.h"
#include "shell.h"
#include "msh_cmd.h"
#include "string.h"

#ifndef FINSH_ARG_MAX
#define FINSH_ARG_MAX    8
#endif 

#define FINSH_FUNCTION_EXPORT_CMD(name, cmd, desc)                      \
	const char __fsym_##cmd##_name[] = #cmd;                            \
	 __attribute__((used)) const struct finsh_syscall __fsym_##cmd __attribute__((section("FSymTab")))= \
    {                                                                   \
		__fsym_##cmd##_name,                                            \
        (syscall_func)&name                                             \
    };

#define FINSH_FUNCTION_EXPORT_ALIAS(name, alias, desc)  \
        FINSH_FUNCTION_EXPORT_CMD(name, alias, desc)


#define MSH_CMD_EXPORT_ALIAS(command, alias, desc)  \
    FINSH_FUNCTION_EXPORT_ALIAS(command, __cmd_##alias, desc)


#define MSH_CMD_EXPORT(command, desc)   \
    FINSH_FUNCTION_EXPORT_CMD(command, __cmd_##command, desc)


int msh_exec(char *cmd, unsigned long length);

void msh_auto_complete(char *prefix);

int msh_exec_module(const char *cmd_line, int size);

int msh_exec_script(const char *cmd_line, int size);

#endif

