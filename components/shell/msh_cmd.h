
/*
 * @file           : msh_cmd.c
 * @autor          : liyou
 * @date           : 2022/01/16
 */

#ifndef __MSH_CMD
#define __MSH_CMD

#include "stdio.h"
#include "msh.h"
#include "task.h"

extern struct finsh_syscall *_syscall_table_begin;
extern struct finsh_syscall *_syscall_table_end;

#if defined(_MSC_VER) || (defined(__GNUC__) && defined(__x86_64__))
struct finsh_syscall* finsh_syscall_next(struct finsh_syscall* call);
struct finsh_sysvar* finsh_sysvar_next(struct finsh_sysvar* call);
#define FINSH_NEXT_SYSCALL(index)  index=finsh_syscall_next(index)
#define FINSH_NEXT_SYSVAR(index)   index=finsh_sysvar_next(index)
#else
#define FINSH_NEXT_SYSCALL(index)  index++
#define FINSH_NEXT_SYSVAR(index)   index++
#endif

typedef void (*syscall_func)(void);

struct finsh_syscall
{
    const char*	name;
    syscall_func 	func;  
};

int msh_help(int argc, char **argv);


#endif
