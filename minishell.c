#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	argc = 0;
	argv = NULL;
	tgetent(NULL, getenv("TERM"));
	bash_loop(env, 0, 0);
}

void	bash_loop(char **env, int ret, int i)
{
	char			c[2];
	t_fullvar		*variables;
	t_completecmd	*complete;

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
	history_loop(&complete, &variables, c, i);
	close(glob_vars.fdout);
}
