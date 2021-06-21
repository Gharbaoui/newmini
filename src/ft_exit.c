/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:35 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:36 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	help_ft_exit(int status, char **args)
{
	if (args[2])
	{
		dup2(2, 1);
		ft_printf(1, "bash: exit: too many arguments\n");
		dup2(1, g_vars.fdout);
		g_vars.exitstatus = 1;
		g_vars.exit = 0;
	}
	else
	{
		status = ft_atoi(args[1]) & 255;
		g_vars.exitstatus = status;
	}
}

int	ft_exit(char **args)
{
	int	status;

	g_vars.exit = 1;
	status = 0;
	if (args[1])
	{
		if (is_number(args[1]) == 0)
		{
			dup2(2, 1);
			ft_printf(3, "bash: exit: ", args[1],
				": numeric argument required\n");
			dup2(1, g_vars.fdout);
			g_vars.exitstatus = 255;
		}
		else
			help_ft_exit(status, args);
	}
	return (g_vars.exitstatus);
}
