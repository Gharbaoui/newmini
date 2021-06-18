/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newexpan.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:32:25 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:32:26 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_full_expanded_line(t_cmd *cmd, t_envs **exenvs)
{
	t_words	*txts;
	char	*cline;
	char	*tline;
	char	*line;
	int		exp;

	if (help1_get_full_expanded(cmd, exenvs, &cline, &exp))
		return ;
	txts = cmd->txts;
	while (txts)
	{
		tline = expand_one_word(txts->txt, exenvs);
		free(txts->txt);
		txts->txt = tline;
		txts = txts->next;
	}
	tline = get_line_from_words_with_spaces(cmd->txts);
	cline = ft_strjoin(&cline, " ");
	line = ft_strjoin(&cline, tline);
	free(tline);
	free_words(&cmd->txts);
	cmd->txts = NULL;
	cline = last_pars(line, &cmd->txts, exp);
	help2_get_full_exp(line, &cmd->command, &cline);
}

int	is_export(char *cmd)
{
	char	*c;

	c = lower_str(cmd);
	if (ft_strcmp(c, "export") == 0)
	{
		free(c);
		return (1);
	}
	free(c);
	return (0);
}

char	*expand_one_word(char *str, t_envs **exenvs)
{
	t_words	*words;
	char	*tmp;

	words = split_by_qout(str);
	var_expan(words, exenvs);
	tmp = get_line_from_words(words);
	free_words(&words);
	return (tmp);
}

void	var_expan(t_words *words, t_envs **exenvs)
{
	char	*help;

	while (words)
	{
		help = get_words_expaned(words->txt, exenvs);
		free(words->txt);
		words->txt = help;
		words = words->next;
	}
}

char	*get_words_expaned(char *str, t_envs **exenvs)
{
	int		totalsize;
	int		i;

	i = -1;
	totalsize = 0;
	if (str[0] == 39)
		return (ft_strdup(str));
	return (double_qout_part(str, exenvs));
}
