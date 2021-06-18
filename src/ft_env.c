/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:32 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:33 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	found = 0;
	envpath = NULL;
	while (help)
	{
		var = get_env(&found, help->txt, vars->exenvs);
		ft_printf(4, var->env_name, "=", var->env_value, "\n");
		help = help->next;
	}
	var = get_env(&found, "PATH", vars->exenvs);
	if (found)
		envpath = get_using_path("/env", var->env_value, &fst);
	if (envpath && *envpath)
		ft_printf(3, "_=", envpath, "\n");
	free(envpath);
	return (0);
}
