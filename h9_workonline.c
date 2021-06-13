#include "minishell.h"

int	fill_wcmd(t_workingcmds **wcmd, t_words *commands, int numofcmds)
{
	int		i;
	t_words	*pip;
	int		ret;

	i = -1;
	if ((help_short_fill_wcmd(wcmd, numofcmds, &pip)) != SUCCESS)
		return (MEMERROR);
	while (++i < numofcmds)
	{
		ret = fill_pips(&pip, commands->txt);
		if (ret != SUCCESS)
		{
			free_words(&pip);
			(*wcmd)->cmds[i] = NULL;
			return (ret);
		}
		(*wcmd)->cmds[i] = pip;
		commands = commands->next;
	}
	(*wcmd)->cmds[i] = NULL;
	return (SUCCESS);
}

int	help_short_fill_wcmd(t_workingcmds **wcmd, int numofcmds, t_words **pipe)
{
	int	i;

	i = -1;
	*wcmd = malloc(sizeof(t_workingcmds));
	if (!(*wcmd))
		return (MEMERROR);
	(*wcmd)->next = NULL;
	(*wcmd)->cmds = malloc(sizeof(t_words *) * (numofcmds + 1));
	if (!(*wcmd)->cmds)
	{
		free(wcmd);
		return (MEMERROR);
	}
	*pipe = NULL;
	return (SUCCESS);
}

int	fill_completecmd(t_completecmd **compcmd, t_words **pips, int pipindex)
{
	int	ret;

	if (pips[pipindex])
	{
		*compcmd = malloc(sizeof(t_completecmd));
		if (!(*compcmd))
			return (0);
		(*compcmd)->splcommand = NULL;
		(*compcmd)->next = NULL;
		ret = fill_pipcmd(&(*compcmd)->splcommand, pips[pipindex]);
		if (ret != SUCCESS)
		{
			free_comp(compcmd);
			*compcmd = NULL;
			return (ret);
		}
		ret = fill_completecmd(&(*compcmd)->next, pips, pipindex + 1);
		if (ret != SUCCESS)
			return (ret);
	}
	return (SUCCESS);
}

int	fill_pipcmd(t_pipcmd **pipcmd, t_words *pip)
{
	int	ret;

	if (pip)
	{
		*pipcmd = malloc(sizeof(t_pipcmd));
		if (!(*pipcmd))
			return (MEMERROR);
		(*pipcmd)->cmd = NULL;
		(*pipcmd)->next = NULL;
		ret = splitlinetowords(pip->txt, &(*pipcmd)->cmd);
		if (ret != SUCCESS)
		{
			free_pipcmd(pipcmd);
			return (ret);
		}
		ret = fill_pipcmd(&(*pipcmd)->next, pip->next);
		if (ret != SUCCESS)
			return (ret);
	}
	return (SUCCESS);
}

int	splitlinetowords(char *str, t_cmd **command)
{
	int		help;
	t_words	*words;

	help = 0;
	words = NULL;
	help = fill_words(&words, str);
	if (help != SUCCESS)
		return (help);
	help = fill_cmdstruct(words, command);
	free_words(&words);
	if (help != SUCCESS)
		return (help);
	return (SUCCESS);
}
