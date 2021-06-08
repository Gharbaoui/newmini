#include "minishell.h"

void	default_fds(int *stdo, int *stdi)
{
	*stdi = dup(0);
	*stdo = dup(1);
}
