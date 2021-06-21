/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   levelbash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:32:03 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:32:04 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	level_of_bash(t_envs **exenvs)
{
	int		found;
	char	*help;
	t_envs	*var;

	var = get_env(&found, "SHLVL", exenvs);
	if (found)
	{
		found = ft_atoi(var->env_value) + 1;
		free(var->env_value);
		help = ft_itoa(found);
		var->env_value = ft_strdup(help);
		free(help);
		return (found);
	}
	return (1);
}
