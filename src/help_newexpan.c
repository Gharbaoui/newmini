#include "minishell.h"

int	help1_get_full_expanded(t_cmd *cmd, t_envs **exenvs, char **cline, int *exp)
{
	char	*help;

	*exp = 0;
	if (cmd->command == NULL)
		return (1);
	help = expand_one_word(cmd->command, exenvs);
	if (!help[0])
	{
		free(cmd->command);
		free(help);
		cmd->command = NULL;
		return (1);
	}
	*exp = is_export(help);
	*cline = help;
	return (0);
}

void	help2_get_full_exp(char *line, char **cmd, char **cline)
{
	free(line);
	free(*cmd);
	*cmd = *cline;
}

int	help1_last_pars(char *line, int i)
{
	int	help;

	help = backslash(line, i);
	if (help % 2 == 0 && line[i] == '"')
		i += get_next_dqpos(line + i);
	else if (help % 2 == 0 && line[i] == 39)
		i += get_next_sqpos(line + i);
	else
		i += get_next_nq(line + i);
	return (i);
}

int	skip_spaces(char *line, int i)
{
	if (*line == 0)
		return (i - 1);
	i++;
	while (line[i] == ' ')
		i++;
	return (i - 1);
}

char	*get_line_from_words(t_words *words)
{
	char	*new_str;
	int		size;
	int		i;
	int		j;

	i = -1;
	size = get_len_ofstrs_in_words(words);
	new_str = malloc(size + 1);
	while (words)
	{
		j = -1;
		while (words->txt[++j])
			new_str[++i] = words->txt[j];
		words = words->next;
	}
	new_str [++i] = 0;
	return (new_str);
}
