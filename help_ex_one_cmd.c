#include "minishell.h"

int	help_run_built_in(t_onecmd cmd, t_fullvar **vars, char *lcmd)
{
	if (ft_strcmp(lcmd, "unset") == 0)
	{
		free(lcmd);
		return (ft_unset(cmd.args, vars));
	}
	if (ft_strcmp(lcmd, "env") == 0)
	{
		free(lcmd);
		return (ft_env(*vars));
	}
	if (ft_strcmp(lcmd, "pwd") == 0)
	{
		free(lcmd);
		return (pwd());
	}
	if (ft_strcmp(lcmd, "exit") == 0)
	{
		free(lcmd);
		return (ft_exit(cmd.args));
	}
	return (-1999);
}

int	run_built_in(t_onecmd cmd, t_fullvar **vars)
{
	char	*lcmd;

	lcmd = lower_str(cmd.cmd);
	if (ft_strcmp(lcmd, "export") == 0)
	{
		free(lcmd);
		return (ft_export(cmd.args, vars));
	}
	if (ft_strcmp(lcmd, "echo") == 0)
	{
		free(lcmd);
		return (_echo(cmd.args));
	}
	if (ft_strcmp(lcmd, "cd") == 0)
	{
		free(lcmd);
		if (ft_strcmp(cmd.args[0], "cd") == 0)
			return (cd(cmd.args, vars));
		return (0);
	}
	return (help_run_built_in(cmd, vars, lcmd));
}
