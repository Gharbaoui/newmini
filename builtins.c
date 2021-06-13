#include "minishell.h"

int	_echo(char **args)
{
	int	new_line;
	int	i;

	glob_vars.exitstatus = 0;
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

	glob_vars.exitstatus = 0;
	if (glob_vars._pwd)
		ft_printf(2, glob_vars._pwd, "\n");
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
		if (glob_vars._pwd)
			free(glob_vars._pwd);
		glob_vars._pwd = ft_strdup(var->env_value);
	}
	else
	{
		if (glob_vars._pwd)
		{
			free(glob_vars._pwd);
		}
		glob_vars._pwd = NULL;
	}
}
