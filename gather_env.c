#include "minishell.h"

int	fill_envtable(t_fullvar **fullvar, char **env)
{
	int	ret;
	int	help;

	help = rest_envt(&(*fullvar)->exenvs);
	if (help == MEMERROR)
		return (MEMERROR);
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
	int		found;
	t_envs	*currentEnv;
	char	*help;
	int		index;
	int		ern;

	help = ft_strdup(line);
	help = origin_var(help);
	currentEnv = make_node_env(&ern, help, vars);
	free(help);
	if (currentEnv == NULL)
		return (SUCCESS);
	if (ern != SUCCESS)
		return (ern);
	index = hash_env_name(currentEnv->env_name);
	add_node_to(&(*vars)->exenvs[index], &currentEnv);
	return (SUCCESS);
}

t_envs	*make_node_env(int *ern, char *line, t_fullvar **vars)
{
	t_envs	*currentEnv;
	int		eq_pos;

	*ern = SUCCESS;
	currentEnv = malloc(sizeof(t_envs));
	if (!currentEnv)
	{
		*ern = MEMERROR;
		return (NULL);
	}
	eq_pos = nlindex(line, '=');
	if (eq_pos != -1)
	{
		if (h1_make_n_env(vars, &currentEnv, eq_pos, line) == 0)
		{
			*ern = MEMERROR;
			return (NULL);
		}
	}
	else
		h2_make_n_env(vars, &currentEnv, line);
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
