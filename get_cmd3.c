#include "minishell.h"

int	key_up(void)
{
	char	*s;
	int		i;

	i = -1;
	while (++i < ft_strlen(glob_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
	}
	i = 1;
	s = tgetstr("ce", NULL);
	write(1, s, ft_strlen(s));
	write(1, glob_vars.navigate->line, ft_strlen(glob_vars.navigate->line));
	if (glob_vars.line)
		free(glob_vars.line);
	glob_vars.line = ft_strdup(glob_vars.navigate->line);
	if (glob_vars.navigate != glob_vars.history)
		glob_vars.navigate2 = glob_vars.navigate;
	if (!glob_vars.navigate->next)
		i = 0;
	else
		glob_vars.navigate = glob_vars.navigate->next;
	return (i);
}
