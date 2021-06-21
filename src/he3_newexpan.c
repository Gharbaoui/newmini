/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   he3_newexpan.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:31:05 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:31:06 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	index_of_dq(char*str, int start)
{
	int	help;

	while (str[++start])
	{
		if (str[start] == '"')
		{
			help = backslash(str, start);
			if (help % 2 == 0)
				return (start);
		}
	}
	return (-1999);
}

char	*get_squto_word(char *str, int *index)
{
	int	nextsq;
	int	i;

	nextsq = nlindex(str + (*index + 1), 39) + (*index + 1);
	i = *index;
	*index = nextsq;
	return (cutstring(str, i, nextsq + 1));
}

char	*get_non_sdquot(char *str, int *index)
{
	int		i;
	int		help;
	char	*tmp;

	i = *index - 1;
	while (str[++i])
	{
		help = backslash(str, i);
		if (help % 2 == 0 && (str[i] == '"' || str[i] == 39))
			break ;
	}
	tmp = cutstring(str, *index, i);
	*index = i - 1;
	return (tmp);
}

char	*last_pars(char *line, t_words **txts, int exp)
{
	t_words	*next;
	char	*cmd;
	int		start;
	int		i;

	i = -1;
	while (*line == ' ')
		line++;
	while (line[++i])
	{
		start = i;
		i = help1_last_pars(line, i);
		cmd = cutstring(line, start, i + 1);
		mk_and_add_to_words(txts, cmd);
		free(cmd);
		i = skip_spaces(line, i);
	}
	delete_backslachs(txts, exp);
	cmd = (*txts)->txt;
	next = (*txts)->next;
	free(*txts);
	*txts = next;
	return (cmd);
}

void	delete_backslachs(t_words **txts, int exp)
{
	t_words	*help;
	char	*newstr;

	help = *txts;
	while (help)
	{
		newstr = remove_back_from_one(help->txt, exp);
		free(help->txt);
		help->txt = newstr;
		help = help->next;
	}
}
