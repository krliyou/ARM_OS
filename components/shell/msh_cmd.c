
/*
 * @file           : msh_cmd.c
 * @autor          : RTT
 * @date           : 2022/01/16
 */

#include "config.h"

#ifdef LY_USING_FINSH

#include "msh_cmd.h"

int msh_help(int argc, char **argv)
{
    printf("LY_OS shell commands:\n\r");
    {
        struct finsh_syscall *index;

        for (index = _syscall_table_begin; 
			index < _syscall_table_end;
			FINSH_NEXT_SYSCALL(index))
        {
            if (ly_strncmp(index->name, "__cmd_", 6) != 0) continue;
            printf("%s \r\n", &index->name[6]);
        }
    }
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(msh_help, __cmd_help, LY_OS shell help.);


static long clear(void)
{
    printf("\x1b[2J\x1b[H");

    return 0;
}
MSH_CMD_EXPORT(clear, clear the terminal screen);

void ver(void)
{
    show_version();
}

MSH_CMD_EXPORT(ver, show LY_OS version information);

#endif
