#include "minishell.h"

int	fill_pips(t_words **pip, char *line)
{
	int	ret;
	int	is_str;
	int	len;
	int	last;

	len = calcfirst(line, '|', &last, &is_str);
	*pip = malloc(sizeof(t_words));
	if (!(*pip))
		return (MEMERROR);
	(*pip)->txt = NULL;
	(*pip)->next = NULL;
	if (!is_str || len == PARSERROR)
		return (PARSERROR);
	if (len >= 0)
	{
		if (help_fill_pipes(pip, line, len) != SUCCESS)
			return (MEMERROR);
		ret = fill_pips(&(*pip)->next, line + len + 1);
		if (ret != SUCCESS)
			return (ret);
	}
	else
		if (help_fill_pipes(pip, line, last) != SUCCESS)
			return (MEMERROR);
	return (SUCCESS);
}

int	help_fill_pipes(t_words **pip, char *line, int len)
{
	(*pip)->txt = cutstring(line, 0, len);
	if (!(*pip)->txt)
	{
		free(*pip);
		*pip = NULL;
		return (MEMERROR);
	}
	(*pip)->next = NULL;
	return (SUCCESS);
}
