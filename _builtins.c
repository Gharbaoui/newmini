#include "minishell.h"

int ft_export(char **args, t_fullvar **vars)
{
	int i;

	i = 0;
	while (args[++i])
	{
		sub_export(vars, args[i]);
	}
	if (i == 1)
		sub_export(vars, NULL);
		
	return 0;
}
