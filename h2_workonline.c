#include "minishell.h"

int	splitby(char *str, int *index)
{
	int	i;
	int	help;

	i = *index;
	help = backslash(str, i);
	if (str[i] == ' ' && help % 2 == 0)
		return (1);
	if ((str[i] == '>' || str[i] == '<') && help % 2 == 0)
	{
		if (i && validchracter(str[i - 1]))
			return (1);
		while (str[i] && (str[i] == '>' || str[i] == '<'))
			i++;
		*index = i;
		return (1);
	}
	return (0);
}

int	valditadsq(char *str)
{
	int	i;
	int	last;

	i = 0;
	last = 0;
	if (str[0] == '"')
	{
		while (str[++i])
		{
			last = backslash(str, i);
			if (i && str[i] == '"' && last % 2 == 0)
				break ;
		}
		if (last % 2 != 0)
			return (PARSERROR);
	}
	else if (str[0] == 39)
	{
		while (str[++i])
			if (str[i] == 39)
				break ;
		if (i > 0 && str[i] != 39)
			return (PARSERROR);
	}
	return (i);
}

int	validchracter(char c)
{
	if (c == '>' || c == '<')
		return (0);
	return (1);
}
