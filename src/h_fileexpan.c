/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_fileexpan.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:52 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:53 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pure_var(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == 39 || str[i] == '"')
			return (0);
	return (1);
}

int	short_creat_w(int *error, char **fs, char *file)
{
	*error = check_name_file(file);
	if (*error)
		*fs = file;
	return (*error);
}

int	check_name_file(char *file)
{
	int	index;

	index = ft_strlen(file) + 1;
	if (file[index] == 'n')
		return (2);
	else if (file[index] == 'a')
		return (3);
	return (0);
}
