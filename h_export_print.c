#include "minishell.h"

int	h1_sortd_merg(t_words **w1, t_words **w2, t_words **fin)
{
	int	ret;

	if ((*w1) == NULL || (*w2) == NULL)
		return (-2);
	ret = ft_strcmp((*w1)->txt, (*w2)->txt);
	if (ret < 0)
	{
		mk_and_add_to_words(fin, (*w1)->txt);
		(*w1) = (*w1)->next;
	}
	else
	{
		mk_and_add_to_words(fin, (*w2)->txt);
		(*w2) = (*w2)->next;
	}
	return (0);
}
