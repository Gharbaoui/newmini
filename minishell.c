#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	argc = 0;
	argv = NULL;
	tgetent(NULL, getenv("TERM"));
	bash_loop(env);
}

void bash_loop(char **env)
{
	char c[2];
	int d;
	char *s;
	int i;
	t_history *history;
	t_history *navigate = NULL;
	t_history *navigate2 = NULL;
	history = NULL;

	signal(SIGINT, handl_sig);
	signal(SIGQUIT, handl_sig);
	t_fullvar *variables;
	int status;
	int ret;
	t_completecmd *complete;
	variables = malloc(sizeof(t_fullvar));
	variables->exenvs = NULL;
	variables->filledvar = NULL;
	variables->allkeys = NULL;
	ret = fill_envtable(&variables, env);// returns Success or memory error or Empty env
	
	add_toenvtable(&variables, "?=0");
	status = 1;
	glob_vars.envchanged = 1;
	// glob_vars.line = malloc(1);
	glob_vars.line = ft_strdup("");
	glob_vars.childruning = 0;
	glob_vars.fdout = dup(1);
	glob_vars.exenvs = &variables->exenvs;
	glob_vars._pwd = NULL;
	update_pwd(variables->exenvs);
	level_of_bash(variables->exenvs);
	// while (status)
	// {
		// free(glob_vars.line);
		// glob_vars.line = NULL;
	// 	write(1, "minishell > ", 12);
	// 	ret = get_next_line(&glob_vars.line);
	// 	// here i need to handel inputs to my bash
	// 	if (ret > 0)
	// 	{
	// 		ret =  workon_line(glob_vars.line, &complete, 0, 0);
	// 		if (ret == PARSERROR)
    //         {
	// 			printf("Parsing Error\n");
    //             free_comp(&complete);
    //         }else{
    //             fullexcute(&complete, &variables);
	// 			if (glob_vars.exit)
	// 			{
	// 				free(glob_vars.line);
	// 				exit(glob_vars.exitstatus);
	// 			}
    //         }
	// 	}else if (ret == 0)
	// 	{
	// 		printf("exited\n");
	// 		status = 0;
	// 	}
	// }
	prompt();
	c[1] = 0;
	while (1)
	{
		d = get_char();
		c[0] = d;
		i = 1;
		if (d >= 32 && d < 127)
		{
			glob_vars.line = ft_strjoin(&glob_vars.line, c);
			write(1, &d, 1);
		}
		else if (d == ENTER)		
		{
			// write(1, glob_vars.line, ft_strlen(glob_vars.line));
			// TODO : check if line is empty , or contains one space , or previous cmd is the same as line.
			// if (check_history(glob_vars.line))
				push_to_history(&history, glob_vars.line);
			write(1, "\n", 1);
			ret =  workon_line(glob_vars.line, &complete, 0, 0);
			if (ret == PARSERROR)
			{
				printf("Parsing Error\n");
                free_comp(&complete);
            }else{
                fullexcute(&complete, &variables);
				if (glob_vars.exit)
				{
					free(glob_vars.line);
					exit(glob_vars.exitstatus);
				}
            }
			prompt();
			if (glob_vars.line)
				free(glob_vars.line);
			glob_vars.line = ft_strdup("");
			navigate = history;
			navigate2 = history;
		}
		else if (d == KEY_UP && i && navigate)
		{
			i = -1;
			while (++i < ft_strlen(glob_vars.line))
			{
				s = tgetstr("le", NULL);
				write(1, s, ft_strlen(s));
			}
			i = 1;
			s = tgetstr("ce", NULL);
			write(1, s, ft_strlen(s));
			write(1, navigate->line, ft_strlen(navigate->line));
			if (glob_vars.line)
				free(glob_vars.line);

			glob_vars.line = ft_strdup(navigate->line);
			if (navigate != history)
				navigate2 = navigate;
			if (!navigate->next)
				i = 0;
			else
				navigate = navigate->next;
		}
		else if (d == KEY_DOWN && i && navigate2)
		{
			if (!navigate2->prev)
				i = 0;
			else
			{
				i = -1;
				while (++i < ft_strlen(glob_vars.line))
				{
					s = tgetstr("le", NULL);
					write(1, s, ft_strlen(s));
				}
				s = tgetstr("ce", NULL);
				write(1, s, ft_strlen(s));
				navigate = navigate2;
				navigate2 = navigate2->prev;
				write(1, navigate2->line, ft_strlen(navigate2->line));
				if (glob_vars.line)
					free(glob_vars.line);
				glob_vars.line = ft_strdup(navigate2->line);
			}
		}
		else if (d == KEY_ERASE)
		{
			if (ft_strlen(glob_vars.line)) // to stop at the prompt if we delete all the line
			{
				s = tgetstr("le", NULL);
				write(1, s, ft_strlen(s));
				s = tgetstr("ce", NULL);
				write(1, s, ft_strlen(s));
				glob_vars.line[ft_strlen(glob_vars.line) - 1] = '\0';
			}
		}
		else if (d == CTRL_RETURN)
		{
			i = -1;
			while (++i < ft_strlen(glob_vars.line))
			{
				s = tgetstr("le", NULL);
				write(1, s, ft_strlen(s));
			}
			s = tgetstr("ce", NULL);
			write(1, s, ft_strlen(s));
			free(glob_vars.line);
			glob_vars.line = ft_strdup("");
		}
		else if (d == CTRL_D)
		{
			if (*glob_vars.line == 0)
			{
				write(1, "exit\n", 5);
				exit(glob_vars.exitstatus);
			}
		}
	}

	close(glob_vars.fdout);
}


