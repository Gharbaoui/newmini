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
		if (g_vars.childruning == 0)
		{
			g_vars.exitstatus = 1;
			update_exit_status(*g_vars.exenvs);
			if (g_vars.line)
				free(g_vars.line);
			g_vars.line = NULL;
			g_vars.line = ft_strdup("");
			write(1, "\n", 1);
			prompt();
		}
		else
			write(1, "\n", 1);
	}
	if (signum == SIGQUIT)
		if (g_vars.childruning)
			write(1, "QUIT 3\n", 7);
}
