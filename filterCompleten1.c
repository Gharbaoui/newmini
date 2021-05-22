#include "minishell.h"

int	addtowords(t_words **words, char *str, int start, int end)
{
	t_words	*help;
	int		len;
	char	*tmp;
	int		i;

	i = -1;
	len = end - start;
	help = malloc(sizeof(t_words));
	if (!help)
		return (0);
	tmp = malloc(len);
	if (!tmp)
	{
		free(help);
		return (0);
	}
	while (++start < end)
		tmp[++i] = str[start];
	tmp[++i] = 0;
	help->txt = tmp;
	help->next = NULL;
	addtmptowords(words, &help);
	return (2);
}

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

char	*cleanWord(t_words *words, int size)
{
	char	*tmp;
	int		i;
	int		j;

	i = -1;
	tmp = malloc(size + 1);
	if (!tmp)
		return (0);
	while (words)
	{
		j = -1;
		while (words->txt[++j])
			tmp[++i] = words->txt[j];
		words = words->next;
	}
	tmp[++i] = 0;
	if (tmp[0] == 0)
	{
		free(tmp);
		tmp = ft_strdup("''");
	}
	return (tmp);
}

void	free_w(t_words **words)
{
	t_words	*next;

	next = (*words)->next;
	while (*words)
	{
		if ((*words)->txt)
			free((*words)->txt);
		free(*words);
		*words = next;
		if (*words)
			next = (*words)->next;
	}	
}
