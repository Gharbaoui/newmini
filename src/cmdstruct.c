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
	t_envs	*pathvar;
	int		found;

	if (pcmd)
	{
		if (pcmd->command)
		{
			pathvar = get_env(&found, "PATH", exenvs);
			if (found)
				fcmd->cmd = get_command(pcmd->command,
						pathvar->env_value, &fcmd->prem, NULL);
			else
				fcmd->cmd = ft_strdup(pcmd->command);
		}
		else
			fcmd->cmd = NULL;
		help_fill_one_cmd(fcmd, pcmd);
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
		allw[++i] = mod_ft_strdup(words->txt, words->file_er);
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
