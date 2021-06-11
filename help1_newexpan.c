#include "minishell.h"

void	sim_h1_get_len(int *tot, int *i)
{
	*tot = 0;
	*i = -1;
}

char	*none_qout_comp(char *line, int exp)
{
	int		i;
	int		j;
	char	*tmp;

	j = -1;
	i = -1;
	tmp = malloc(ft_strlen(line) + 1);
	while (line[++i])
	{
		if (line[i] == 92 && is_special_in_none(line[i + 1]))
		{
			if (!(exp && line[i + 1] == '$'))
				i++;
		}
		else if (line[i] == 92)
			i++;
		tmp[++j] = line[i];
	}
	tmp[++j] = 0;
	return (tmp);
}

int	get_next_dqpos(char *line)
{
	int	dqfound;
	int	i;
	int	help;

	i = 0;
	dqfound = 0;
	while (line[++i])
	{
		help = backslash(line, i);
		if (dqfound % 2 != 0 && line[i] == 39)
			i += get_next_sqpos(line + i);
		else if (line[i] == '"')
		{
			if (help % 2 == 0)
				dqfound += 1;
		}
		else if (dqfound % 2 != 0 && line[i] == ' ' && help % 2 == 0)
			break ;
	}
	return (i - 1);
}	

int	get_next_sqpos(char *line)
{
	int	i;
	int	sqfound;
	int	help;

	i = 0;
	sqfound = 0;
	while (line[++i])
	{
		if (sqfound % 2 != 0 && line[i] == '"')
			i += get_next_dqpos(line + i);
		else if (line[i] == 39)
			sqfound += 1;
		else if (sqfound % 2 != 0 && line[i] == ' ')
		{
			help = backslash(line, i);
			if (help % 2 == 0)
				break ;
		}
	}
	return (i - 1);
}

int	get_next_nq(char *line)
{
	int	i;
	int	help;

	i = 0;
	while (line[++i])
	{
		help = backslash(line, i);
		if (help % 2 == 0)
		{
			if (line[i] == '"')
				i += get_next_dqpos(line + i);
			else if (line[i] == 39)
				i += get_next_sqpos(line + i);
			else if (line[i] == ' ')
				break ;
		}
	}
	return (i - 1);
}
