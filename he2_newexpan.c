#include "minishell.h"

char	*remove_back_from_one(char *line, int exp)
{
	t_words	*w;
	t_words	*head;
	char	*fin;

	w = split_by_qout(line);
	head = w;
	while (w)
	{
		fin = finl_cost_back(w->txt, exp);
		free(w->txt);
		w->txt = fin;
		w = w->next;
	}
	fin = get_line_from_words(head);
	free_words(&head);
	return (fin);
}

char	*finl_cost_back(char *line, int exp)
{
	char	c;

	c = line[0];
	if (c == 39 || c == '"')
	{
		line++;
		line[ft_strlen(line) - 1] = 0;
	}
	if (c == 39)
		return (ft_strdup(line));
	else if (c == '"')
	{
		return (double_quot_comp(line, exp));
	}
	return (none_qout_comp(line, exp));
}

char	*double_quot_comp(char *line, int exp)
{
	char	*tmp;
	int		i;
	int		j;

	j = -1;
	i = -1;
	tmp = malloc(ft_strlen(line) + 1);
	while (line[++i])
	{
		if (line[i] == 92 && is_special_in_double(line[i + 1]) && !exp)
			i++;
		tmp[++j] = line[i];
	}
	tmp[++j] = 0;
	return (tmp);
}

int	is_special_in_double(char c)
{
	if (c == 92 || c == '`' || c == '"')
		return (1);
	if (c == '$')
		return (1);
	return (0);
}

int	is_special_in_none(char c)
{
	if (is_special_in_double(c))
		return (1);
	if (c == '>' || c == '<' || c == '&' || c == '*')
		return (1);
	if (c == '|' || c == ']' || c == '[' || c == 39)
		return (1);
	if (c == '?' || c == '}' || c == '{' || c == ' ')
		return (1);
	if (c == ';' || c == ':' || c == '/')
		return (1);
	if (c == '!' || c == '`' || c == '#')
		return (1);
	return (0);
}

