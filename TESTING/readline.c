/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 17:24:23 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/16 17:28:49 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
int	get_next_line(char **line)
{
	static char	*rest;
	char		*current;
	int			help;

	if (error(line, &rest, &current))
		return -1;
	help = validate(line, &rest, &current);
	if (help == 1)
		return help;
	while ((help = read(0, current, BUFFER_SIZE)) > 0)
	{
		current[help] = 0;
		help = nlindex(current, '\n');
		if (help != -1)
			return  complete(line, &rest, &current, help); // free current
		rest = ft_strjoin(&rest, current);
	}
	return last_line(line, &rest, &current);
}

int complete(char **line, char **rest, char **current, int index)
{
	char *first;

	first = split(*current, 0, index);
	*line = ft_strjoin(rest, first); // rest freed
	free (first);
	*rest = split(*current, 1, index);
	free(*current); // free current
	return 1;
}

int validate(char **line, char **rest, char **current)
{
	int nl_pos;
	char *last;

	nl_pos = nlindex(*rest, '\n');
	if (nl_pos == -1)
		return -3;
	*line = split(*rest, 0, nl_pos);
	last = split(*rest, 1, nl_pos);
	free (*rest);
	free(*current);
	*rest = last;
	return 1;
}

int last_line(char **line, char **rest, char **current)
{
	*line = *rest;
	*rest = NULL;
	free (*current);
	return 0;
}
*/

char *split(char *current, int val, int index) // return  memory
{
	char *tmp;
	int i;
	int len;

	i = -1;
	if (!val)
	{
		len = index + 1;
		if (!(tmp = malloc(len)))
			return 0;
		while (++i < index)
			tmp[i] = current[i];
		tmp[i] = 0;
	}else{
		len = ft_strlen (current) - index;
		if (!(tmp = malloc(len)))
			return 0;
		len = index;
		while (current[++index])
			tmp[++i] = current[index];
		tmp[++i] = 0;
	}
	return tmp;
}
/*
int error(char **line, char **rest, char **current)
{
	if (!line)
		return  -1;
	if (!(*current = malloc(BUFFER_SIZE + 1)))
		return -1;
	if (!(*rest))
	{
		if (!(*rest = malloc(1)))
		{
			free(*current);
			return -1;
		}
		(*rest)[0] = 0;
	}
	return 0;
}
*/
int  ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return i;
}

int nlindex(char *str, char c)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return i;
	return -1;
}

// char *ft_strjoin(char **rest, char *current) // free rest and not current and return other memory (tmp, current) return memory
// {
// 	char *tmp;
// 	int len;
// 	int i;

// 	i = -1;
// 	len = ft_strlen (*rest) + ft_strlen(current) + 1;
// 	if (!(tmp = malloc(len)))
// 		return 0;
// 	while ((*rest)[++i])
// 		tmp[i] = (*rest)[i];
// 	len = -1;
// 	while (current[++len])
// 		tmp[i + len] = current[len];
// 	free (*rest);
// 	tmp[i + len] = 0;
// 	return tmp;
// }

char *ft_strjoin(char **rest, char *current) // free rest and not current and return other memory (tmp, current) return memory
{
	char *tmp;
	int len;
	int i;

	i = -1;
	len = ft_strlen (*rest) + ft_strlen(current) + 1;
	if (!(tmp = malloc(len)))
		return 0;
	while ((*rest)[++i])
		tmp[i] = (*rest)[i];
	len = -1;
	while (current[++len])
		tmp[i + len] = current[len];
	free (*rest);
	tmp[i + len] = 0;
	return tmp;
}
