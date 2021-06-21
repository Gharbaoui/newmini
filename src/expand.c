/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:10 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:11 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipcommand	*expand_current_command(t_completecmd *cucmd, t_fullvar *envs)
{
	if (cucmd)
	{
		expand_full_pipcmd(&cucmd->splcommand, envs->exenvs);
		return (get_cmd_struct(cucmd->splcommand, envs->exenvs));
	}
	return (NULL);
}

int	expand_full_pipcmd(t_pipcmd **pipcmd, t_envs **exenvs)
{
	t_pipcmd	*cupipcmd;

	cupipcmd = *pipcmd;
	while (cupipcmd)
	{
		expand_one_cmdstrct(&cupipcmd->cmd, exenvs);
		cupipcmd = cupipcmd->next;
	}
	return (SUCCESS);
}

int	expand_one_cmdstrct(t_cmd **cmd, t_envs **exenvs)
{
	get_full_expanded_line(*cmd, exenvs);
	return (SUCCESS);
}

int	get_var_name(char *line, char **key)
{
	int	i;

	i = -1;
	if (line[0] == '?')
	{
		*key = ft_strdup("?");
		return (SUCCESS);
	}
	while (line[++i])
		if (is_special(line[i]) || line[i] == ' '
			|| line[i] == '\t' || line[i] == '=')
			break ;
	*key = cutstring(line, 0, i);
	return (SUCCESS);
}

int	mk_and_add_to_words(t_words **words, char *line)
{
	t_words	*cuw;

	cuw = malloc(sizeof(t_words));
	cuw->txt = ft_strdup(line);
	cuw->next = NULL;
	addtmptowords(words, &cuw);
	return (SUCCESS);
}
