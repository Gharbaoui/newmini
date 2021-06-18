#include "minishell.h"

void	expan_files(t_words **files, t_envs **exenvs)
{
	int		ret;
	t_words	*txts;
	char	*file;

	txts = (*files);
	while (txts)
	{
		file = expand_one_word(txts->txt, exenvs);
		edit_file_name(&file, txts->txt);
		free(txts->txt);
		txts->txt = file;
		txts = txts->next;
	}
}

void	edit_file_name(char **file, char *origin)
{
	char	*predict;

	predict = remove_back_from_one(*file, 0);
	if (*predict == 0)
	{
		*file = change_file_according(origin, *file);
		free(predict);
		return ;
	}
	else if (is_not_valid(*file))
	{
		free(predict);
		free(*file);
		*file = ft_strdup("a b");
	}
	free(*file);
	*file = wrap_name(predict);
	free(predict);
}

char	*wrap_name(char *name)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = malloc(ft_strlen(name) + 3);
	tmp[i] = '"';
	while (name[++i])
		tmp[i] = name[i - 1];

	tmp[i] = name[i - 1];
	tmp[i + 1] = '"';
	tmp[i + 2] = 0;
	return (tmp);
}

int	is_not_valid(char *line)
{
	int		i;
	int		start;
	char	*cmd;
	t_words *txts;
	
	i = -1;
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
	if (txts->next)
		return (1);
	return (0);
}

char	*change_file_according(char *origin, char *file)
{
	int ret;

	free(file);
	ret = is_pure_var(origin);
	if (ret)
		file = ft_strdup("a b");
	else
		file = ft_strdup("");
	return (file);
}

int	is_pure_var(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == 39 || str[i] == '"')
			return (0);
	return (1);
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
