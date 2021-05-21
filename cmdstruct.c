#include "minishell.h"

t_pipcommand	*get_cmd_struct(t_pipcmd *cmd, t_envs **exenvs)
{
	t_pipcommand	*headcmd;

	headcmd = malloc(sizeof(t_pipcommand));
	fill_onepipcmd(headcmd, cmd, exenvs);
	return (headcmd);
}

int	fill_onepipcmd(t_pipcommand *pipcmd, t_pipcmd *pip, t_envs **exenvs)
{
	while (pip)
	{
		fill_one_cmd(&pipcmd->cmd, pip->cmd, exenvs);
		if (pip->next)
			pipcmd->next = malloc(sizeof(t_pipcommand));
		else
			pipcmd->next = NULL;
		pipcmd = pipcmd->next;
		pip = pip->next;
	}
	return (SUCCESS);
}

int	fill_one_cmd(t_onecmd *fcmd, t_cmd *pcmd, t_envs **exenvs)
{
	if (pcmd)
	{
		h1_fill_one_cmd(fcmd, pcmd, exenvs);
		fcmd->args = transfrm_ln_arr(pcmd->txts, pcmd->command, 1);
		if (fcmd->args == NULL && fcmd->cmd)
		{
			fcmd->args = malloc(sizeof(char *) * 2);
			fcmd->args[0] = ft_strdup(pcmd->command);
			fcmd->args[1] = NULL;
		}
		fcmd->files = transfrm_ln_arr(pcmd->files, pcmd->command, 0);
		fcmd->ops = transfrm_ln_arr(pcmd->ops, pcmd->command, 0);
	}
	else
		fcmd = NULL;
	return (SUCCESS);
}

char	**transfrm_ln_arr(t_words *words, char *cmd, int iscmd)
{
	char	**allw;
	int		len;
	int		i;

	if (words == NULL)
		return (NULL);
	len = how_many_words(words);
	allw = malloc(sizeof(char *) * (len + 1 + iscmd));
	i = -1;
	if (iscmd)
		allw[++i] = ft_strdup(cmd);
	while (words)
	{
		allw[++i] = ft_strdup(words->txt);
		words = words->next;
	}
	allw[++i] = NULL;
	return (allw);
}

int	how_many_words(t_words *words)
{
	int	i;

	i = 0;
	while (words)
	{
		words = words->next;
		i++;
	}
	return (i);
}
