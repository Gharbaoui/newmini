#include "minishell.h"

int	fill_envtable(t_fullvar **fullvar, char **env)
{
	int	ret;
	int	help;

	help = rest_envt(&(*fullvar)->exenvs);
	if (help == MEMERROR)
		return (MEMERROR);
	help = -1;
	while (env[++help])
	{
		ret = add_toenvtable(fullvar, env[help]);
		if (ret != SUCCESS)
		{
			free_env(&(*fullvar)->exenvs);
			free ((*fullvar)->exenvs);
			(*fullvar)->exenvs = NULL;
			return (MEMERROR);
		}
	}
	if (help == 0)
	{
		free ((*fullvar)->exenvs);
		return (EMPTYENV);
	}
	return (SUCCESS);
}

int	add_toenvtable(t_fullvar **vars, char *line)
{
	t_envs	*current_env;
	int		index;
	int		ern;

	current_env = make_node_env(&ern, line, vars);
	if (current_env == NULL)
		return (SUCCESS);
	if (ern != SUCCESS)
		return (ern);
	index = hash_env_name(current_env->env_name);
	add_node_to(&(*vars)->exenvs[index], &current_env);
	return (SUCCESS);
}

t_envs	*make_node_env(int *ern, char *line, t_fullvar **vars)
{
	t_envs	*currentEnv;

	*ern = SUCCESS;
	currentEnv = malloc(sizeof(t_envs));
	if (!currentEnv)
	{
		*ern = MEMERROR;
		return (NULL);
	}
	if (help_make_node_env(vars, &currentEnv, line, ern))
		return (NULL);
	return (currentEnv);
}

int	ft_exist(t_words *words, char *str)
{
	while (words)
	{
		if (!ft_strcmp(words->txt, str))
			return (1);
		words = words->next;
	}
	return (0);
}

int	check_envvar(char *line, int eq_pos)
{
	int	i;

	i = 0;
	if (!ft_isalpha(line[0]) && line[0] != '_')
		return (ENVERROR);
	while (++i < eq_pos - 1)
	{
		if (!ft_isalpha(line[i]) && !ft_isdigit(line[i]) && line[i] != '_')
			return (ENVERROR);
	}
	if ((i < eq_pos) && !ft_isalpha(line[i]) && !ft_isdigit(line[i])
		&& line[i] != '_' && line[i] != '+')
		return (ENVERROR);
	return (SUCCESS);
}
