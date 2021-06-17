#include "minishell.h"

int	_echo(char **args)
{
	int	new_line;
	int	i;

	g_vars.exitstatus = 0;
	new_line = 0;
	i = 1;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
		i++;
	new_line = i;
	while (args[i])
	{
		ft_printf(1, args[i]);
		i++;
		if (args[i])
			ft_printf(1, " ");
	}
	if (new_line == 1 || !(args[1]))
		ft_printf(1, "\n");
	return (0);
}

int	pwd(void)
{
	char	cwd[128];

	g_vars.exitstatus = 0;
	if (g_vars._pwd)
		ft_printf(2, g_vars._pwd, "\n");
	else if (getcwd(cwd, sizeof(cwd)))
		ft_printf(2, cwd, "\n");
	else
		ft_printf(1, "PWD");
	return (0);
}

void	update_pwd(t_envs **exenvs)
{
	int		found;
	t_envs	*var;

	var = get_env(&found, "PWD", exenvs);
	if (found)
	{
		if (g_vars._pwd)
			free(g_vars._pwd);
		g_vars._pwd = ft_strdup(var->env_value);
	}
	else
	{
		if (g_vars._pwd)
		{
			free(g_vars._pwd);
		}
		g_vars._pwd = NULL;
	}
}
