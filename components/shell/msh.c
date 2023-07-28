
/*
 * @file           : msh.c
 * @autor          : RTT
 * @date           : 2022/01/16
 */

#include "config.h"

#ifdef LY_USING_FINSH

#include "msh.h"

typedef int (*cmd_function_t)(int argc, char **argv);

static int msh_split(char *cmd, unsigned long length, char *argv[FINSH_ARG_MAX])
{
    char *ptr;
    unsigned long position;
    unsigned long  argc;
    unsigned long  i;

    ptr = cmd;
    position = 0;
    argc = 0;

    while (position < length)
    {
        while ((*ptr == ' ' || *ptr == '\t') && position < length)
        {
            *ptr = '\0';
            ptr ++;
            position ++;
        }

        if (argc >= FINSH_ARG_MAX)
        {
            printf("Too many args ! We only Use:\n");
            for (i = 0; i < argc; i++)
            {
                printf("%s ", argv[i]);
            }
            printf("\n\r");
            break;
        }

        if (position >= length) break;

        if (*ptr == '"')
        {
            ptr ++;
            position ++;
            argv[argc] = ptr;
            argc ++;

            while (*ptr != '"' && position < length)
            {
                if (*ptr == '\\')
                {
                    if (*(ptr + 1) == '"')
                    {
                        ptr ++;
                        position ++;
                    }
                }
                ptr ++;
                position ++;
            }
            if (position >= length) break;

            *ptr = '\0';
            ptr ++;
            position ++;
        }
        else
        {
            argv[argc] = ptr;
            argc ++;
            while ((*ptr != ' ' && *ptr != '\t') && position < length)
            {
                ptr ++;
                position ++;
            }
            if (position >= length) break;
        }
    }

    return argc;
}


static cmd_function_t msh_get_cmd(char *cmd, int size)
{
    struct finsh_syscall *index;
    cmd_function_t cmd_func = NULL;

    for (index = _syscall_table_begin;
            index < _syscall_table_end;
            FINSH_NEXT_SYSCALL(index))
    {
        if (ly_strncmp(index->name, "__cmd_", 6) != 0) continue;

        if (ly_strncmp(&index->name[6], cmd, size) == 0 &&
                index->name[6 + size] == '\0')
        {
            cmd_func = (cmd_function_t)index->func;
            break;
        }
    }

    return cmd_func;
}

static int _msh_exec_cmd(char *cmd, unsigned long length, int *retp)
{
    int argc;
    unsigned long cmd0_size = 0;
    cmd_function_t cmd_func;
    char *argv[FINSH_ARG_MAX];

    while (cmd0_size < length && (cmd[cmd0_size] != ' ' && cmd[cmd0_size] != '\t'))
        cmd0_size ++;

    if (cmd0_size == 0)
        return -1;

    cmd_func = msh_get_cmd(cmd, cmd0_size);
    if (cmd_func == NULL)
        return -1;

    ly_memset(argv, 0x00, sizeof(argv));
    argc = msh_split(cmd, length, argv);
    if (argc == 0)
        return -1;

    *retp = cmd_func(argc, argv);
    return 0;
}

int msh_exec(char *cmd, unsigned long length)
{
    int cmd_ret;

    while ((length > 0) && (*cmd  == ' ' || *cmd == '\t'))
    {
        cmd++;
        length--;
    }

    if (length == 0)
        return 0;
    if (_msh_exec_cmd(cmd, length, &cmd_ret) == 0)
    {
        return cmd_ret;
    }

    {
        char *tcmd;
        tcmd = cmd;
        while (*tcmd != ' ' && *tcmd != '\0')
        {
            tcmd++;
        }
        *tcmd = '\0';
    }

	printf("%s: command not found.\r\n", cmd);
    return -1;
}

#ifdef FINSH_AUTO_COMPLETE

static int str_common(const char *str1, const char *str2)
{
    const char *str = str1;

    while ((*str != 0) && (*str2 != 0) && (*str == *str2))
    {
        str ++;
        str2 ++;
    }

    return (str - str1);
}

void msh_auto_complete(char *prefix)
{
    int length, min_length;
    const char *name_ptr, *cmd_name;
    struct finsh_syscall *index;

    min_length = 0;
    name_ptr = NULL;

    {
        for (index = _syscall_table_begin; index < _syscall_table_end; FINSH_NEXT_SYSCALL(index))
        {
            if (ly_strncmp(index->name, "__cmd_", 6) != 0) continue;

            cmd_name = (const char *) &index->name[6];
            if (ly_strncmp(prefix, cmd_name, strlen(prefix)) == 0)
            {
                if (min_length == 0)
                {
                    /* set name_ptr */
                    name_ptr = cmd_name;
                    /* set initial length */
                    min_length = strlen(name_ptr);
                }

                length = str_common(name_ptr, cmd_name);
                if (length < min_length)
                    min_length = length;

                printf("%s\r\n", cmd_name);
            }
        }
    }

    /* auto complete string */
    if (name_ptr != NULL)
    {
        ly_strncpy(prefix, name_ptr, min_length);
    }

    return ;
}
#endif

#endif


