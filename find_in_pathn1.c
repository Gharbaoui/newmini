#include "minishell.h"

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

char	*get_using_path(char *cmd, char *PATH, struct stat *fst)
{
	char	*cpt;
	int		index;
	int		status;

	status = 1;
	while (*PATH)
	{
		cpt = get_one_path(PATH, &index);
		cpt = ft_strjoin(&cpt, cmd);
		if (stat(cpt, fst) == 0)
			return (cpt);
		free(cpt);
		PATH += index;
		if (*PATH == ':')
			PATH++;
	}
	return (NULL);
}

char	*get_one_path(char *PATH, int *index)
{
	int		cp;
	char	*tmp;

	cp = nlindex(PATH, ':');
	if (cp == -1)
		cp = ft_strlen(PATH);
	tmp = cutstring(PATH, 0, cp);
	*index = cp;
	return (tmp);
}
