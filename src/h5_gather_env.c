/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h5_gather_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:30:44 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:30:45 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*double_qout_part(char *str, t_envs **exenvs)
{
	int		help;
	char	*tmp;
	int		i;
	int		j;

	i = -1;
	j = -1;
	help = get_len_double_qout(str, exenvs);
	tmp = malloc(help + 1);
	while (str[++i])
	{
		help = backslash(str, i);
		if (help % 2 == 0 && str[i] == '$' && str[i + 1])
			i += fill_part_of_str(tmp, &j, str + i, exenvs);
		else
			tmp[++j] = str[i];
	}
	tmp[++j] = 0;
	return (tmp);
}

int	fill_part_of_str(char *tmp, int *index, char *str, t_envs **exenvs)
{
	char	*key;
	int		help;
	int		j;
	int		i;
	t_envs	*var;

	j = -1;
	i = *index;
	get_var_name(str + 1, &key);
	var = get_env(&help, key, exenvs);
	if (help)
	{
		while (var->env_value[++j])
			tmp[++i] = var->env_value[j];
	}
	*index = i;
	j = ft_strlen(key);
	free(key);
	return (j);
}
