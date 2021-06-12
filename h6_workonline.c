#include "minishell.h"

int	check_newlines(char *line)
{
	int	help;
	int	i;

	i = -1;
	while (line[++i])
	{
		help = h1_check_newlines(&line, line, &i);
		if (help != SUCCESS)
			return (help);
	}
	return (SUCCESS);
}

int	h1_check_newlines(char **line, char *backup, int *i)
{
	int	help;
	int	bqnum;
	int	last;
	int	is_str;

	is_str = 0;
	help = calcfirst(*line, 92, &last, &is_str);
	bqnum = backslash(backup, *line - backup + help);
	if (help != -2 && help != -1 && bqnum % 2 == 0)
	{
		(*line) += help + 1;
		*i = -1;
		if (is_empty(*line))
			return (PARSERROR);
	}
	else
	{
		(*line)++;
		*i = -1;
	}
	return (SUCCESS);
}

int	is_empty(char *line)
{
	int	i;

	if (line[0])
		return (0);
	return (1);
}

void	rest(t_workingcmds **wcmd, t_words **commands)
{
	*wcmd = NULL;
	*commands = NULL;
}

void	rest_txt_next(char **str, t_words **next)
{
	*next = NULL;
	*str = NULL;
}
