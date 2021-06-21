/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileexpan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:23 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:24 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expan_files(t_words **files, t_envs **exenvs)
{
	t_words	*txts;
	char	*file;

	txts = (*files);
	while (txts)
	{
		file = expand_one_word(txts->txt, exenvs);
		edit_file_name(&file, txts->txt, &txts->file_er);
		free(txts->txt);
		txts->txt = file;
		txts = txts->next;
	}
}

void	edit_file_name(char **file, char *origin, int *ern)
{
	char	*predict;

	*ern = 0;
	predict = remove_back_from_one(*file, 0);
	if (*predict == 0)
	{
		*file = change_file_according(origin, *file, ern);
		free(predict);
		return ;
	}
	else if (is_not_valid(*file))
	{
		free(predict);
		free(*file);
		*file = ft_strdup(origin);
		*ern = 3;
		return ;
	}
	free(*file);
	*file = predict;
}

int	is_not_valid(char *line)
{
	int		i;
	int		start;
	char	*cmd;
	t_words	*txts;

	i = -1;
	txts = NULL;
	while (*line == ' ')
		line++;
	while (line[++i])
	{
		start = i;
		i = help1_last_pars(line, i);
		cmd = cutstring(line, start, i + 1);
		mk_and_add_to_words(&txts, cmd);
		free(cmd);
		i = skip_spaces(line, i);
	}
	i = 0;
	if (txts->next)
		i = 1;
	if (txts)
		free_words(&txts);
	return (i);
}

char	*change_file_according(char *origin, char *file, int *ern)
{
	int	ret;

	free(file);
	ret = is_pure_var(origin);
	if (ret)
	{
		*ern = 3;
		file = ft_strdup(origin);
	}
	else
	{
		*ern = 2;
		file = ft_strdup("");
	}
	return (file);
}

char	*remove_spaces(char *filename)
{
	int		j;
	int		i;
	char	*file;

	i = -1;
	j = ft_strlen(filename);
	while (filename[++i])
		if (filename[i] != ' ')
			break ;
	while (--j >= 0)
		if (filename[j] != ' ')
			break ;
	if (filename[i])
		file = cutstring(filename, i, j + 1);
	else
		file = ft_strdup("");
	free(filename);
	return (file);
}
