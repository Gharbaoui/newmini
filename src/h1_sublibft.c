#include "minishell.h"

int	istxt(char c)
{
	if (c != '<' && c != '>')
		return (1);
	return (0);
}

void	free_dstr(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
}

int	opvalid(char *str)
{
	if (ft_cmpstr(str, ">") || ft_cmpstr(str, "<") || ft_cmpstr(str, ">>"))
		return (1);
	return (0);
}

int	ft_cmpstr(char *s1, char *s2)
{
	int	i;

	i = -1;
	while (s1[++i])
	{
		if (s1[i] != s2[i])
			return (0);
	}
	if (ft_strlen(s2 + i))
		return (0);
	return (1);
}

int	is_special(char c)
{
	if (c == 92 || c == '>' || c == '<' || c == '\'')
		return (1);
	if (c == '$' || c == '"' || c == '&')
		return (1);
	if (c == '|' || c == ']' || c == '[')
		return (1);
	if (c == '?' || c == '}' || c == '{')
		return (1);
	if (c == ';' || c == ':' || c == '/')
		return (1);
	if (c == '!' || c == '`' || c == '#')
		return (1);
	return (0);
}
