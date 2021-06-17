#include "minishell.h"

int	get_len_double_qout(char *str, t_envs **exenvs)
{
	int		i;
	t_envs	*var;
	int		totalsize;
	char	*key;
	int		help;

	sim_h1_get_len(&totalsize, &i);
	while (str[++i])
	{
		if (str[i] == '$')
		{
			help = backslash(str, i);
			if (help % 2 == 0)
			{
				get_var_name(str + i + 1, &key);
				i += ft_strlen(key);
				var = get_env(&help, key, exenvs);
				if (help)
					totalsize += ft_strlen(var->env_value)
						- (ft_strlen(key) + 1);
				free(key);
			}
		}
	}
	return (i + totalsize);
}

char	*get_line_from_words_with_spaces(t_words *words)
{
	char	*tmp;
	int		size;
	int		i;
	int		j;

	i = -1;
	size = get_len_ofstrs_in_words(words);
	tmp = malloc(size + 1);
	while (words)
	{
		j = -1;
		while (words->txt[++j])
			tmp[++i] = words->txt[j];
		words = words->next;
		if (words)
			tmp[++i] = ' ';
	}
	tmp [++i] = 0;
	return (tmp);
}

int	get_len_ofstrs_in_words(t_words *words)
{
	int	total;

	total = 0;
	while (words)
	{
		total += ft_strlen(words->txt) + 1;
		words = words->next;
	}
	return (total);
}

t_words	*split_by_qout(char *str)
{
	int		i;
	int		bst;
	char	*help;
	t_words	*w;

	w = NULL;
	i = -1;
	while (str[++i])
	{
		bst = backslash(str, i);
		if (bst % 2 == 0 && (str[i] == '"' || str[i] == 39))
		{
			if (str[i] == '"')
				help = get_quot_word(str, &i);
			else if (str[i] == 39)
				help = get_squto_word(str, &i);
		}
		else
			help = get_non_sdquot(str, &i);
		mk_and_add_to_words(&w, help);
		free(help);
	}
	return (w);
}

char	*get_quot_word(char *str, int *index)
{
	int	i;
	int	nextquot;

	i = *index;
	nextquot = index_of_dq(str, *index);
	*index = nextquot;
	return (cutstring(str, i, nextquot + 1));
}
