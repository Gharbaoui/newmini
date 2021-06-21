/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h1_workonline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:30:28 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:30:29 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_wcmd(t_workingcmds **wcmd, int numofcmds)
{
	int	i;

	i = -1;
	while (++i < numofcmds)
	{
		if ((*wcmd)->cmds[i])
			free_words(&(*wcmd)->cmds[i]);
	}
	if ((*wcmd)->cmds)
		free((*wcmd)->cmds);
	if (*wcmd)
		free(*wcmd);
}

void	free_words(t_words **words)
{
	t_words	*next;

	if (*words)
		next = (*words)->next;
	while (*words)
	{
		if ((*words)->txt)
		{
			free((*words)->txt);
		}
		free (*words);
		*words = next;
		if (*words)
			next = (*words)->next;
	}
}

void	free_cmdstr(t_cmd **cmd)
{
	if (*cmd)
	{
		if ((*cmd)->command)
			free((*cmd)->command);
		if ((*cmd)->txts)
			free_words(&(*cmd)->txts);
		if ((*cmd)->files)
			free_words(&(*cmd)->files);
		if ((*cmd)->ops)
			free_words(&(*cmd)->ops);
		free(*cmd);
	}
}

void	free_pipcmd(t_pipcmd **pipcmd)
{
	t_pipcmd	*next;

	if (*pipcmd)
		next = (*pipcmd)->next;
	while (*pipcmd)
	{
		free_cmdstr(&(*pipcmd)->cmd);
		free(*pipcmd);
		*pipcmd = next;
		if (*pipcmd)
			next = (*pipcmd)->next;
	}
}

void	free_comp(t_completecmd **cmp)
{
	t_completecmd	*next;

	if (*cmp)
		next = (*cmp)->next;
	while (*cmp)
	{
		free_pipcmd(&(*cmp)->splcommand);
		free(*cmp);
		*cmp = next;
		if (*cmp)
			next = (*cmp)->next;
	}
}
