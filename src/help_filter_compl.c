/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_filter_compl.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:31:19 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:31:20 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	addtmptowords(t_words **words, t_words **word)
{
	t_words	*help;

	(*word)->next = NULL;
	if (!(*words))
		*words = *word;
	else
	{
		help = *words;
		while (help && help->next)
			help = help->next;
		help->next = *word;
	}	
}

int	modify_ln(t_words **words)
{
	t_words	*help;

	help = *words;
	while (help)
	{
		if (!(modify_str(&help->txt)))
			return (MEMERROR);
		help = help->next;
	}
	return (SUCCESS);
}
