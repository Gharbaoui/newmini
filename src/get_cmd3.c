#include "minishell.h"

int	key_up(void)
{
	char	*s;
	int		i;

	i = -1;
	while (++i < ft_strlen(g_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
	}
	i = 1;
	s = tgetstr("ce", NULL);
	write(1, s, ft_strlen(s));
	write(1, g_vars.navigate->line, ft_strlen(g_vars.navigate->line));
	if (g_vars.line)
		free(g_vars.line);
	g_vars.line = ft_strdup(g_vars.navigate->line);
	if (g_vars.navigate != g_vars.history)
		g_vars.navigate2 = g_vars.navigate;
	if (!g_vars.navigate->next)
		i = 0;
	else
		g_vars.navigate = g_vars.navigate->next;
	return (i);
}
