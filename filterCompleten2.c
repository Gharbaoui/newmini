#include "minishell.h"

int	modify_str(char **str)
{
	t_words	*words;
	int		size;
	int		num;
	int		i;
	int		check;

	words = NULL;
	if (!(*str))
		return (2);
	i = -1;
	size = 0;
	words = h1_modify_str(*str, &size, &check);
	free(*str);
	*str = cleanWord(words, size);
	if (check == 5 || !(*str))
	{
		if (words)
			free_w(&words);
		return (MEMERROR);
	}
	if (words)
		free_w(&words);
	return (SUCCESS);
}
