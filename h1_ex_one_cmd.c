#include "minishell.h"

int h1_r_built(char *lcmd, t_onecmd cmd, t_fullvar **vars)
{
	int ret;

	ret = -1999;
	if (ft_strcmp(lcmd, "export") == 0)
		ret = ft_export(cmd.args, vars);
	else if (ft_strcmp(lcmd, "echo") == 0)
		ret = _echo(cmd.args);
	else if (ft_strcmp(lcmd, "cd") == 0)
	{
		if (ft_strcmp(cmd.args[0], "cd") == 0)
			ret = cd(cmd.args, vars);
		else
			ret = 0;
	}
	else if (ft_strcmp(lcmd, "unset") == 0)
		ret = ft_unset(cmd.args, vars);
	return ret;	
}
