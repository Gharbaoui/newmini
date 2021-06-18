/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h3_workonline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:30:37 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:30:38 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	backslash(char *line, int index)
{
	int	total;

	total = 0;
	while (--index >= 0 && line[index] == 92)
		total++;
	return (total);
}

int	check_errors(int ern, t_words **words)
{
	if (ern != SUCCESS)
	{
		free_words(words);
		return (ern);
	}
	return (0);
}

int	simplecheck(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ')
			return (0);
	}
	if (i == ft_strlen(line))
		return (1);
	if (i == 1)
		return (1);
	return (0);
}

int	fill_cmd_objs(t_words **txts, char *str)
{
	t_words	*help;

	if (!(*txts))
	{
		if (h1_fill_cmd_objs(txts, str))
			return (0);
	}
	else
	{
		help = *txts;
		while (help->next)
			help = help->next;
		help->next = malloc(sizeof(t_words));
		if (!(help)->next)
			return (0);
		help = help->next;
		help->txt = ft_strdup(str);
		if (!(help)->txt)
			return (0);
		help->next = NULL;
	}
	return (1);
}

int	h1_fill_cmd_objs(t_words **txts, char *str)
{
	*txts = malloc(sizeof(t_words));
	if (!(*txts))
		return (MEMERROR);
	(*txts)->next = NULL;
	(*txts)->txt = ft_strdup(str);
	if (!(*txts)->txt)
		return (1);
	return (0);
}
