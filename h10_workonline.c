#include "minishell.h"

int	fill_cmdstruct(t_words *words, t_cmd **command)
{
	int		ret;
	char	*firstword;

	if (!reset_command(command))
		return (0);
	if (words)
	{
		firstword = words->txt;
		ret = h1_fill_cmdstruct(&words, command, firstword);
		if (ret != 10)
		{
			if (ret != SUCCESS)
				return (ret);
		}
		else
		{
			return (h2_fill_cmdstruct(&words, command, firstword, 0));
		}
	}
	return (SUCCESS);
}

int	h1_fill_cmdstruct(t_words **words, t_cmd **command, char *firstword)
{
	int	ret;

	if (istxt(firstword[0]))
	{
		if (!(*command)->command)
		{
			(*command)->command = ft_strdup(firstword);
			if (!((*command)->command))
				return (MEMERROR);
		}
		else
		{
			if (!fill_command(command, firstword, 1))
				return (MEMERROR);
		}
		ret = fill_cmdstruct((*words)->next, command);
		if (ret != SUCCESS)
			return (ret);
		return (SUCCESS);
	}
	return (10);
}

int	h2_fill_cmdstruct(t_words **words,
	t_cmd **command, char *firstword, int ret)
{
	if (opvalid(firstword))
	{
		if (!(fill_command(command, firstword, 3)))
			return (MEMERROR);
		*words = (*words)->next;
		if (*words)
			firstword = (*words)->txt;
		else
			return (PARSERROR);
		if (istxt(firstword[0]))
		{
			if (!(fill_command(command, firstword, 2)))
				return (MEMERROR);
			ret = fill_cmdstruct((*words)->next, command);
			if (ret != SUCCESS)
				return (ret);
			return (SUCCESS);
		}
		else
			return (PARSERROR);
	}
	else
		return (PARSERROR);
}
