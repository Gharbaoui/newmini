#include "minishell.h"

void	help_fill_one_cmd(t_onecmd *fcmd, t_cmd *pcmd)
{
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
