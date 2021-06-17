#include "minishell.h"

int	calcfirst(char *line, char c, int *last, int *is_str)
{
	int	i;
	int	help;
	int	dq;
	int	sq;

	*is_str = 0;
	if (help_short_calcfirst(&i, &dq, &sq, 1) && line[0] == '"')
		dq++;
	while (line[++i])
	{
		help = backslash(line, i);
		if (help % 2 == 0 || sq % 2 != 0)
		{
			if (line[i] == '"' && sq % 2 == 0)
				dq += h1_calcfirst(i);
			else if (line[i] == 39 && dq % 2 == 0)
				sq++;
			if (line[i] == c && (dq % 2 == 0 && sq % 2 == 0))
				return (i);
			if (line[i] != ' ' && line[i] != '|')
				*is_str = 1;
		}
	}
	*last = i;
	return (help_short_calcfirst(&i, &dq, &sq, 0));
}

int	h1_calcfirst(int i)
{
	if (i > 0)
		return (1);
	return (0);
}

int	help_short_calcfirst(int *i, int *dq, int *sq, int is_first)
{
	if (is_first == 1)
	{
		*i = -1;
		*dq = 0;
		*sq = 0;
	}
	else
	{	
		if (*dq % 2 != 0 || *sq % 2 != 0)
			return (PARSERROR);
		return (-2);
	}
	return (10);
}

int	countnumberofcmds(t_words *commands)
{
	int		i;
	int		check;
	t_words	*last;

	i = -1;
	check = 0;
	while (commands)
	{
		last = commands;
		if (commands->next)
			if (simplecheck(commands->txt))
				return (PARSERROR);
		commands = commands->next;
		i++;
	}
	help_short_count(last->txt, &i);
	return (i);
}

void	help_short_count(char *help, int *i)
{
	int	check;
	int	j;

	j = -1;
	if (!help)
		return ;
	check = 0;
	while (help[++j])
	{
		if (help[j] != ' ')
		{
			check = 1;
			break ;
		}
	}
	if (check)
		(*i)++;
}
