#include "minishell.h"


int len_of_words(t_words *words)
{
	int total;

	total = 0;
	while (words)
	{
		total += ft_strlen(words->txt);
		if (words->txt[0] == '"' || words->txt[0] == 39)
			total -= 2;
		words = words->next;
	}
	return total;
}

char *from_words_to_line(t_words *words)
{
	int len;
	char *tmp;
	char c;
	int i;
	int j;

	i = -1;
	len = len_of_words(words);
	tmp = malloc(len + 1);
	while (words)
	{
		j = -1;
		c = words->txt[0];
		if (c == '"' || c == 39)
		{
			words->txt++;
			words->txt[ft_strlen(words->txt) - 1] = 0;
		}
		while (words->txt[++j])
			tmp[++i] = words->txt[j];
		if (c == '"' || c == 39)
			words->txt--;
		words = words->next;
	}
	tmp[++i] = 0;
	return  tmp;
}



int ch_was_var(t_words **words)
{
	char *line;
	
	line = from_words_to_line(*words);
	if (nlindex(line, '=') != -1)
	{
		free_words (words);
		*words = malloc(sizeof(t_words));
		(*words)->txt = line;
		(*words)->next = NULL;
		return 1;
	}
	free(line);
	return 0;
}

int var_founded(t_words **words, int kl, int counter)
{

}

int var_exist(char *line, int *len)
{
	int i;
	int ret;

	i = -1;
	ret = 0;
	if (line[0] == '"' || line[0] == 39)
		return 0;
	while (line[++i])
	{
		if (line[i] == '=')
			ret = 1;
	}
	*len += i;
	return ret;
}
