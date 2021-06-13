#include "minishell.h"

void	help_ft_exit(int status, char **args)
{
	if (args[2])
	{
		dup2(2, 1);
		ft_printf(1, "bash: exit: too many arguments\n");
		dup2(1, glob_vars.fdout);
		glob_vars.exitstatus = 1;
		glob_vars.exit = 0;
	}
	else
	{
		status = ft_atoi(args[1]) & 255;
		glob_vars.exitstatus = status;
	}
}

int	ft_exit(char **args)
{
	int	status;

	glob_vars.exit = 1;
	status = 0;
	if (args[1])
	{
		if (is_number(args[1]) == 0)
		{
			dup2(2, 1);
			ft_printf(3, "bash: exit: ", args[1],
				": numeric argument required\n");
			dup2(1, glob_vars.fdout);
			glob_vars.exitstatus = 2;
		}
		else
			help_ft_exit(status, args);
	}
	return (glob_vars.exitstatus);
}
