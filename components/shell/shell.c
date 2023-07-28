
/*
 * @file           : shell.c
 * @autor          : RTT
 * @date           : 2022/01/16
 */

#include "config.h"

#ifdef LY_USING_FINSH

#include "shell.h"
#include "msh.h"

struct finsh_shell shell;

struct finsh_syscall *_syscall_table_begin = NULL;
struct finsh_syscall *_syscall_table_end   = NULL;

#define finsh_getchar() get_str()

#ifdef FINSH_USING_AUTH

static unsigned long finsh_set_password(const char *password, int pw_len)
{
	if (pw_len < FINSH_PASSWORD_MIN || pw_len > FINSH_PASSWORD_MAX)
		return -1;

    ly_strncpy(shell.password, password, FINSH_PASSWORD_MAX);

    return 0;
}

static void finsh_wait_auth(void)
{
    int ch;
    int input_finish = 0;
    char password[FINSH_PASSWORD_MAX] = { 0 };
    unsigned long cur_pos = 0;
	
	if (finsh_set_password(FINSH_PASSWORD, \
			(sizeof(FINSH_PASSWORD) -1)) != 0)
	{
		printf("Finsh password set failed.\n\r");
		return;
	}

    while (1)
    {
		printf("Password for login: ");
        while (!input_finish)
        {
            while (1)
            {
                ch = (int)finsh_getchar();

                if (ch >= ' ' && ch <= '~' && cur_pos < FINSH_PASSWORD_MAX)
                {
                    printf("*");
                    password[cur_pos++] = ch;
                }
                else if (ch == '\b' && cur_pos > 0)
                {
                    cur_pos--;
                    password[cur_pos] = '\0';
                    printf("\b \b");
                }
                else if (ch == '\r' || ch == '\n')
                {
                    printf("\r\n");
                    input_finish = 1;
                    break;
                }
            }
        }

        if (!strncmp(shell.password, password, FINSH_PASSWORD_MAX)) return;
        else
        {
			delay_ms(1000);
            printf("Sorry, try again.\r\n");
            cur_pos = 0;
            input_finish = 0;
            ly_memset(password, '\0', FINSH_PASSWORD_MAX);
        }
    }
}
#endif

#ifdef FINSH_AUTO_COMPLETE
static void shell_auto_complete(char *prefix)
{
    printf("\r\n");
	msh_auto_complete(prefix);
    printf("%s%s", FINSH_PROMPT, prefix);
}
#endif

#ifdef FINSH_USING_HISTORY
static int shell_handle_history(struct finsh_shell *shell)
{
#if defined(_WIN32)
    int i;
    printf("\r");

    for (i = 0; i <= 60; i++)
        putchar(' ');
	printf("\r");
#else
    printf("\033[2K\r");
#endif
    
	printf("%s%s", FINSH_PROMPT, shell->line);

    return 0;
}

static void shell_push_history(struct finsh_shell *shell)
{
    if (shell->line_position != 0)
    {
        if (shell->history_count >= FINSH_HISTORY_LINES)
        {
            if (ly_memcmp(&shell->cmd_history[FINSH_HISTORY_LINES - 1], 	\
								shell->line, FINSH_CMD_SIZE))
            {
                int index;
                for (index = 0; index < FINSH_HISTORY_LINES - 1; index ++)
                {
                    ly_memcpy(&shell->cmd_history[index][0],
                           &shell->cmd_history[index + 1][0], FINSH_CMD_SIZE);
                }
                ly_memset(&shell->cmd_history[index][0], 0, FINSH_CMD_SIZE);
                ly_memcpy(&shell->cmd_history[index][0], shell->line, shell->line_position);

                shell->history_count = FINSH_HISTORY_LINES;
            }
        }
        else
        {
            if (shell->history_count == 0 || 	\
				ly_memcmp(&shell->cmd_history[shell->history_count - 1], 	\
				shell->line, FINSH_CMD_SIZE))
            {
                shell->current_history = shell->history_count;

                ly_memset(&shell->cmd_history	\
				[shell->history_count][0], 0, FINSH_CMD_SIZE);
                ly_memcpy(&shell->cmd_history[shell->history_count][0], \
				shell->line, shell->line_position);
                shell->history_count ++;
            }
        }
    }
		shell->current_history = shell->history_count;
}
#endif

void finsh_shell_entry()
{
    int ch;

	finsh_system_init();

#ifdef FINSH_USING_AUTH    
	finsh_wait_auth();
#endif
	
	printf("%s", FINSH_PROMPT);

    while (1)
    {
        ch = (int)finsh_getchar();

        if (ch == 0x1b)
        {
            shell.stat = WAIT_SPEC_KEY;
            continue;
        }
        else if (shell.stat == WAIT_SPEC_KEY)
        {
            if (ch == 0x5b)
            {
                shell.stat = WAIT_FUNC_KEY;
                continue;
            }

            shell.stat = WAIT_NORMAL;
        }
        else if (shell.stat == WAIT_FUNC_KEY)
        {
            shell.stat = WAIT_NORMAL;

            if (ch == 0x41)  
            {
#ifdef FINSH_USING_HISTORY
                 
                if (shell.current_history > 0)
                    shell.current_history --;
                else
                {
                    shell.current_history = 0;
                    continue;
                }

                ly_memcpy(shell.line, &shell.cmd_history[shell.current_history][0],
                       FINSH_CMD_SIZE);
                shell.line_curpos = shell.line_position = (unsigned short)strlen(shell.line);
                shell_handle_history(&shell);
#endif
                continue;
            }
            else if (ch == 0x42) 
            {
#ifdef FINSH_USING_HISTORY
                
                if (shell.current_history < shell.history_count - 1)
                    shell.current_history ++;
                else
                {
                     
                    if (shell.history_count != 0)
                        shell.current_history = shell.history_count - 1;
                    else
                        continue;
                }

                ly_memcpy(shell.line, &shell.cmd_history[shell.current_history][0],
                       FINSH_CMD_SIZE);
                shell.line_curpos = shell.line_position = (unsigned short)strlen(shell.line);
                shell_handle_history(&shell);
#endif
                continue;
            }
            else if (ch == 0x44) 
            {
                if (shell.line_curpos)
                {
					printf("\b");
                    shell.line_curpos --;
                }

                continue;
            }
            else if (ch == 0x43)  
            {
                if (shell.line_curpos < shell.line_position)
                {
					printf("%c", shell.line[shell.line_curpos]);
                    shell.line_curpos ++;
                }

                continue;
            }
        }

        if (ch == '\0' || ch == 0xFF) 
			continue;
        
        else if (ch == '\t')
        {
#ifdef FINSH_AUTO_COMPLETE
            int i;

            for (i = 0; i < shell.line_curpos; i++)
                printf("\b");
  
            shell_auto_complete(&shell.line[0]);
            shell.line_curpos = shell.line_position = (unsigned short)strlen(shell.line);
#endif
            continue;
        }

        else if (ch == 0x7f || ch == 0x08)
        {
            if (shell.line_curpos == 0)
                continue;

            shell.line_position--;
            shell.line_curpos--;

            if (shell.line_position > shell.line_curpos)
            {
                int i;

                ly_memmove(&shell.line[shell.line_curpos],
                           &shell.line[shell.line_curpos + 1],
                           shell.line_position - shell.line_curpos);
                shell.line[shell.line_position] = 0;

                printf("\b%s  \b", &shell.line[shell.line_curpos]);

                for (i = shell.line_curpos; i <= shell.line_position; i++)
                    printf("\b");
            }
            else
            {
                printf("\b \b");
                shell.line[shell.line_position] = 0;
            }

            continue;
        }

        if (ch == '\r' || ch == '\n')
        {
#ifdef FINSH_USING_HISTORY
            shell_push_history(&shell);
#endif        
            printf("\r\n");
            msh_exec(shell.line, shell.line_position);
            printf("%s", FINSH_PROMPT);
            ly_memset(shell.line, 0, sizeof(shell.line));
            shell.line_curpos = shell.line_position = 0;
            continue;
        }

        if (shell.line_position >= FINSH_CMD_SIZE)
            shell.line_position = 0;

        if (shell.line_curpos < shell.line_position)
        {
            int i;

            ly_memmove(&shell.line[shell.line_curpos + 1],
                       &shell.line[shell.line_curpos],
                       shell.line_position - shell.line_curpos);
            shell.line[shell.line_curpos] = ch;
            printf("%s", &shell.line[shell.line_curpos]);

            for (i = shell.line_curpos; i < shell.line_position; i++)
                printf("\b");
        }
        else
        {
            shell.line[shell.line_position] = ch;
            printf("%c", ch);
        }

        ch = 0;
        shell.line_position ++;
        shell.line_curpos++;
        if (shell.line_position >= FINSH_CMD_SIZE)
        {
            shell.line_position = 0;
            shell.line_curpos = 0;
        }
    } 
}

int finsh_system_init(void)
{	
	#if defined(__CC_ARM) || defined(__CLANG_ARM)   
		extern const int FSymTab$$Base;
		extern const int FSymTab$$Limit;
		_syscall_table_begin = (struct finsh_syscall*) &FSymTab$$Base;
		_syscall_table_end = (struct finsh_syscall*) &FSymTab$$Limit;
	#endif
	
    return 0;
}

#endif


