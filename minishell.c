#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	argc = 0;
	argv = NULL;
	tgetent(NULL, getenv("TERM"));
	bash_loop(env, 0, 0, 0);
}

void bash_loop(char **env, int ret, int i, int d)
{
	char c[2];
	char *s;
	t_fullvar *variables;
	t_completecmd *complete;

	signal(SIGINT, handl_sig);
	signal(SIGQUIT, handl_sig);
	variables = malloc(sizeof(t_fullvar));
	variables->exenvs = NULL;
	variables->filledvar = NULL;
	variables->allkeys = NULL;
	ret = fill_envtable(&variables, env);
	
	
	add_toenvtable(&variables, "?=0");
	glob_vars.envchanged = 1;
	glob_vars.line = ft_strdup("");
	glob_vars.childruning = 0;
	glob_vars.fdout = dup(1);
	glob_vars.exenvs = &variables->exenvs;
	glob_vars._pwd = NULL;
	update_pwd(variables->exenvs);
	level_of_bash(variables->exenvs);
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
			key_enter(&complete, &variables);
		else if (d == KEY_UP && i && glob_vars.navigate)
			i = key_up();
		else if (d == KEY_DOWN && i && glob_vars.navigate2)
			i = key_down();
		else if (d == KEY_ERASE)
			key_erase();
		else if (d == CTRL_RETURN)
			ctrl_return();
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


