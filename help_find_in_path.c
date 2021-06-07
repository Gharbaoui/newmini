#include "minishell.h"

int	help_get_command(char *cmd, int ret, char **command)
{
	int	p;

	p = 0;
	if (ret == 1)
	{
		*command = ft_strdup(cmd);
		p = get_cmd_state(*command);
	}
	else
		*command = lower_str(cmd);
	return (p);
}

int	help2_get_command(char *lcmd, char **command, char *path)
{
	struct stat	fst;
	char		*bcmd;
	char		*b;
	int			p;

	b = ft_strdup("/");
	bcmd = ft_strjoin(&b, lcmd);
	*command = get_using_path(bcmd, path, &fst);
	if (*command)
		p = is_excutable(fst);
	else
	{
		*command = ft_strdup(lcmd);
		p = 0;
	}
	free(bcmd);
	return (p);
}

int	get_cmd_state(char *cmd)
{
	struct stat	fst;

	if (stat(cmd, &fst) == 0)
		return (is_excutable(fst));
	return (0);
}

char	lower_char(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}

char	*lower_str(char *str)
{
	char	*tmp;
	int		i;

	i = -1;
	tmp = malloc(ft_strlen(str) + 1);
	while (str[++i])
		tmp[i] = lower_char(str[i]);
	tmp[i] = 0;
	return (tmp);
}
