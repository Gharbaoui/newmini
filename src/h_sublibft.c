#include "minishell.h"

char	*mod_ft_strdup(char *str, int ern)
{
	char	*tmp;
	int		len;
	int		i;

	i = -1;
	len = ft_strlen(str) + 2;
	tmp = malloc(len);
	while (str[++i])
		tmp[i] = str[i];
	tmp[i] = 0;
	if (ern == 0)
		tmp[i + 1] = 't';
	else if (ern == 2)
		tmp[i + 1] = 'n';
	else
		tmp[i + 1] = 'a';
	return (tmp);
}

int	file_error(int err, char **fs)
{
	dup2(2, 1);
	if (err == 3)
		ft_printf(3, "bash: ", fs[0], ": ambiguous redirect\n");
	else
		ft_printf(3, "bash: ", fs[0], ": No such file or directory\n");
	g_vars.exitstatus = 1;
	dup2(1, g_vars.fdout);
	free(fs);
	return (1);
}
