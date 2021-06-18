/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h5_workonline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:30:46 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:30:47 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_words(t_words **words, char *str)
{
	int	start;
	int	i;
	int	help;

	i = -1;
	start = 0;
	help = 0;
	while (*str && *str == ' ')
		str++;
	if (str[0])
	{
		help = h1_fill_words(words, str, &i, help);
		if (help != SUCCESS)
			return (help);
		(*words)->txt = cutstring(str, 0, i);
		if (!((*words)->txt))
		{
			free(*words);
			return (MEMERROR);
		}
		fill_words(&(*words)->next, str + i);
	}
	return (SUCCESS);
}

int	h1_fill_words(t_words **words, char *str, int *index, int help)
{
	int	i;

	i = -1;
	*words = malloc(sizeof(t_words));
	if (!(*words))
		return (MEMERROR);
	(*words)->next = NULL;
	while (str[++i])
	{
		help = backslash(str, i);
		if (help % 2 == 0 && (str[i] == '"' || str[i] == 39))
		{
			help = valditadsq(str + i);
			if (help < 0)
				return (PARSERROR);
			i += help;
		}
		else if (splitby(str, &i))
			break ;
	}
	*index = i;
	return (SUCCESS);
}
