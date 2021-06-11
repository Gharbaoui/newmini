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
		if (!(*txts = malloc(sizeof(t_words))))
			return (MEMERROR); // memory failure
		(*txts)->next = NULL;
		if (!((*txts)->txt = ft_strdup(str)))
			return (0);
	}
	else
	{
		help  = *txts;
		while (help->next)
			help = help->next;
		if (!(help->next = malloc(sizeof(t_words))))
			return (0);
		help = help->next;
		if (!(help->txt = ft_strdup(str)))
			return (0);
		help->next = NULL;
	}
	return (1);
}
