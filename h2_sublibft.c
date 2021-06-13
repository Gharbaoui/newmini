#include "minishell.h"

int	reset_command(t_cmd **command)
{
	if (!(*command))
	{
		*command = malloc(sizeof(t_cmd));
		if (!(*command))
		{
			*command = NULL;
			return (0);
		}
		(*command)->command = NULL;
		(*command)->txts = NULL;
		(*command)->files = NULL;
		(*command)->ops = NULL;
	}
	return (1);
}

char	*ft_strdup(char *str)
{
	char	*tmp;
	int		len;

	len = ft_strlen(str) + 1;
	tmp = malloc(len);
	if (!tmp)
		return (0);
	len = -1;
	while (str[++len])
		tmp[len] = str[len];
	tmp[len] = 0;
	return (tmp);
}

void	free_laststr(t_pipcommand **pcmd)
{
	t_pipcommand	*help;

	help = *pcmd;
	while (help)
	{
		free_onecmd(&help->cmd);
		help = help->next;
	}
	*pcmd = NULL;
}

void	free_pipes(int **pipes, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		free(pipes[i]);
	free(pipes);
}

void	free_onecmd(t_onecmd *cmd)
{
	if (cmd)
	{
		if ((cmd)->cmd)
			free((cmd)->cmd);
		if ((cmd)->args)
		{
			free_dstr((cmd)->args);
			free(cmd->args);
		}
		if ((cmd)->files)
		{
			free_dstr((cmd)->files);
			free_dstr((cmd)->ops);
			free(cmd->files);
			free(cmd->ops);
		}
		free(cmd);
		cmd = NULL;
	}
}
