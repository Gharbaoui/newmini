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

int	modify_str(char **str)
{
	t_words	*words;

	if (*str == NULL)
		return (SUCCESS);
	words = split_by_qout(*str);
	free(*str);
	*str = get_line_from_words(words);
	free_words(&words);
	return (SUCCESS);
}

int	addtowords(t_words **words, char *str, int start, int end)
{	
	t_words	*help;
	int		len;
	char	*tmp;
	int		i;

	i = -1;
	len = end - start;
	help = malloc(sizeof(t_words));
	if (!help)
		return (0);
	tmp = malloc(len);
	if (!tmp)
	{
		free(help);
		return (0);
	}
	while (++start < end)
		tmp[++i] = str[start];
	tmp[++i] = 0;
	help->txt = tmp;
	help->next = NULL;
	addtmptowords(words, &help);
	return (2);
}
