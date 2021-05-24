#include "minishell.h"

int level_of_bash(t_envs **exenvs)
{
	int found;
	t_envs *var;

	var = get_env(&found, "SHLVL", exenvs);
	if (found)
	{
		found = ft_atoi(var->env_value) + 1;
		free(var->env_value);
		var->env_value = ft_strdup(ft_itoa(found));
		return found;
	}
	return 1;
}
