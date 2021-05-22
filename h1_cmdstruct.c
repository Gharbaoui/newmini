#include "minishell.h"

void	h1_fill_one_cmd(t_onecmd *fcmd, t_cmd *pcmd, t_envs **exenvs)
{
	t_envs	*pathvar;
	int		found;

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
}
