#include "minishell.h"

int	rest_envt(t_envs ***envtable)
{
	int	i;

	if (!((*envtable) = malloc(sizeof(t_envs *) * ENVSIZE)))
		return (MEMERROR);
	i = -1;
	while (++i < ENVSIZE)
	{
		(*envtable)[i] = NULL;
	}
	return (-1);
}

int	hash_env_name(char *env_name)
{
	int	sum;
	int	i;

	i = -1;
	sum = 0;
	while (env_name[++i])
		sum += env_name[i];
	sum *= env_name[i - 1];
	return (sum % ENVSIZE);
}

void	free_env(t_envs ***envtable)
{
	int	i;

	i = -1;
	while (++i < ENVSIZE)
	{
		if ((*envtable)[i])
			free_envlist(&(*envtable)[i]);
	}
}

void	free_envlist(t_envs **envlist)
{
	t_envs	*next;

	if (*envlist)
		next = (*envlist)->next;
	while (*envlist)
	{
		free_one_env(envlist);
		*envlist = next;
		if (*envlist)
			next = (*envlist)->next;
	}	
}

void	free_one_env(t_envs **oneenv)
{
	if (*oneenv)
	{
		if ((*oneenv)->env_name)
			free((*oneenv)->env_name);
		if ((*oneenv)->env_value)
			free((*oneenv)->env_value);
		free(*oneenv);
	}
}
