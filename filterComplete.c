#include "minishell.h"

int	filter_complete(t_completecmd **completecmd)
{	
	t_completecmd	*help;
	int				ret;

	help = *completecmd;
	while (help)
	{
		ret = filter_pipcmd1(&(help)->splcommand);
		if (ret != SUCCESS)
			return (ret);
		help = help->next;
	}
	return (SUCCESS);
}

int	filter_pipcmd1(t_pipcmd **pipcmd)
{
	t_pipcmd	*help;
	int			ret;

	help = *pipcmd;
	while (help)
	{
		ret = filter_cmd(&(help)->cmd);
		if (ret != SUCCESS)
			return (ret);
		help = help->next;
	}
	return (SUCCESS);
}

int	filter_cmd(t_cmd **cmd)
{
	t_cmd	*help;
	int		check;
	int		ret;

	check = 0;
	help = *cmd;
	if (!(modify_str(&help->command)))
		return (MEMERROR);
	if (!(modify_ln(&help->txts)))
		return (MEMERROR);
	if (!(modify_ln(&help->files)))
		return (MEMERROR);
	return (SUCCESS);
}

int	filter_check_envvar(t_words *txts)
{
	int	ret;
	int	eq_pos;

	while (txts)
	{
		eq_pos = nlindex(txts->txt, '=');
		if (eq_pos == -1)
			eq_pos = ft_strlen(txts->txt);
		ret = check_envvar(txts->txt, eq_pos);
		if (ret != SUCCESS)
			return (PARSERROR);
		txts = txts->next;
	}
	return (SUCCESS);
}

int	help_in_modstr(char *line, int i, char c)
{
	int	num;

	if (c == 39)
	{
		if (line[i] == 39)
			return (0);
	}
	else
	{
		num = backslash(line, i);
		if (line[i] == '"' && num % 2 == 0)
			return (0);
	}
	return (1);
}
