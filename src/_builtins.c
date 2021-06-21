/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _builtins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ghar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:01:18 by mel-ghar          #+#    #+#             */
/*   Updated: 2021/06/18 21:20:24 by mel-ghar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(char **args, t_fullvar **vars)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (args[++i])
	{
		ret = sub_export(vars, args[i]);
	}
	if (i == 1)
		ret = sub_export(vars, NULL);
	return (ret);
}
