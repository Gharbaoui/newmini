#include "minishell.h"

int	ft_env (t_fullvar *vars)
{
	t_envs		*var;
	struct stat	fst;
	t_words		*help;
	char		*envpath;
	int			found;

	help = vars->filledvar;
	g_vars.exitstatus = 0;
	while (help)
	{
		var = get_env(&found, help->txt, vars->exenvs);
		ft_printf(3, var->env_name, var->env_value, "\n");
		help = help->next;
	}
	var = get_env(&found, "PATH", vars->exenvs);
	if (found)
		envpath = get_using_path("/env", var->env_value, &fst);
	ft_printf(3, "_=", envpath, "\n");
	free(envpath);
	return (0);
}
