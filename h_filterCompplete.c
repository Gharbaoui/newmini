#include "minishell.h"

t_words *h1_modify_str(char *line, int *s, int *check)
{
	int i;
	int start;
	int num;
	t_words *words;

	words = NULL;
	i = -1;
	*s = 0;
	while (line[++i])
	{
		num = backslash(line, i);
		if (num % 2 == 0 && (line[i] == '"' || line[i] == 39))
			h_h1_mod_str(line, &i, &start, s);
		else
			*check = h2_h1_mod_str(line, &i, &start, s);
		if (!(addtowords(&words, line, start, i)))
		{
			*check = 5;
			break;
		}
		h3_h1_mod_str(&i, check, &start);
	}
	return words;
}

void h_h1_mod_str(char *line, int *i, int *start, int *s)
{
	int check;

	check = 0;
	*start = *i;
	while (line[++(*i)] && help_in_modstr(line, *i, line[*i]))
		check = 1; // number has no meaning
	if (*i - *start > 1)
	{
		(*start)--;
		(*i)++;
	}
	*s += *i - *start - 1;
}

int h2_h1_mod_str(char *line, int *i, int *start, int *s)
{
	int check;

	*start = *i - 1;
	while (line[++(*i)] && (line[*i] != '"' && line[*i] != 39))
		check = 1;
	*s += *i - *start - 1;
	check = 3;
	return check;
}

void h3_h1_mod_str(int *i, int *check, int *start)
{
	if (*check == 3)
	{
		*check = 0;
		(*i)--;
	}
	else if (*i - *start > 1)
		(*i)--;
}
