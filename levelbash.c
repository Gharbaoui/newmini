#include "minishell.h"

int level_of_bash(t_envs **exenvs)
{
	int found;
	char *help;
	t_envs *var;

	var = get_env(&found, "SHLVL", exenvs);
	if (found)
	{
		found = ft_atoi(var->env_value) + 1;
		free(var->env_value);
		help = ft_itoa(found);
		var->env_value = ft_strdup(help);
		free(help);
		return found;
	}
	return 1;
}
