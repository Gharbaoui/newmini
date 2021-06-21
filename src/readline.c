/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 17:24:23 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:32:32 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*split(char *current, int val, int index)
{
	char	*tmp;
	int		i;
	int		len;

	i = -1;
	if (!val)
		tmp = beg_str(current, index);
	else
	{
		len = ft_strlen (current) - index;
		tmp = malloc(len);
		if (!tmp)
			return (0);
		len = index;
		while (current[++index])
			tmp[++i] = current[index];
		tmp[++i] = 0;
	}
	return (tmp);
}

char	*beg_str(char *current, int index)
{
	int		len;
	char	*tmp;
	int		i;

	i = -1;
	len = index + 1;
	tmp = malloc(len);
	if (!tmp)
		return (NULL);
	while (++i < index)
		tmp[i] = current[i];
	tmp[i] = 0;
	return (tmp);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	nlindex(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (-1);
}

char	*ft_strjoin(char **rest, char *current)
{
	char	*tmp;
	int		len;
	int		i;

	i = -1;
	len = ft_strlen (*rest) + ft_strlen(current) + 1;
	tmp = malloc(len);
	if (!tmp)
		return (0);
	while ((*rest)[++i])
		tmp[i] = (*rest)[i];
	len = -1;
	while (current[++len])
		tmp[i + len] = current[len];
	free (*rest);
	tmp[i + len] = 0;
	return (tmp);
}
