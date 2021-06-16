#include "minishell.h"

int	get_status(int st)
{
	int	ret;

	if (WIFSIGNALED(st))
		ret = WTERMSIG(st) + 128;
	else
		ret = WEXITSTATUS(st);
	return (ret);
}

void	handl_sig(int signum)
{
	if (signum == SIGINT)
	{
		if (glob_vars.childruning == 0)
		{
			glob_vars.exitstatus = 1;
			update_exit_status(*glob_vars.exenvs);
			if (glob_vars.line)
				free(glob_vars.line);
			glob_vars.line = NULL;
			glob_vars.line = ft_strdup("");
			write(1, "\n", 1);
			prompt();
		}
		else
			write(1, "\n", 1);
	}
	if (signum == SIGQUIT)
		if (glob_vars.childruning)
			write(1, "QUIT 3\n", 7);
}
