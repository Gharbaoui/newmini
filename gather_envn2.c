#include "minishell.h"

t_envs	*look_inln(int *found, char *env_name, t_envs *lnenv)
{
	int		ret;
	t_envs	*cur;
	t_envs	*next;

	*found = 0;
	cur = NULL;
	next = lnenv->next;
	while (lnenv)
	{
		ret = ft_cmpstr(env_name, lnenv->env_name);
		if (ret == 1)
		{
			*found = 1;
			break ;
		}
		lnenv = next;
		if (lnenv)
			next = lnenv->next;
	}
	if (*found == 1)
		cur = lnenv;
	return (cur);
}

int	delete_env(t_envs ***table, char *env_name, int *founded)
{
	int	index;

	index = hash_env_name(env_name);
	if ((*table)[index])
	{
		delete_exactfromln(&(*table)[index], env_name, founded);
	}
	return (SUCCESS);
}

int	delete_exactfromln(t_envs **list, char *key, int *found)
{
	t_envs	*next;
	t_envs	*prev;

	*found = 0;
	next = *list;
	if (next && ft_cmpstr(key, next->env_name))
	{
		*found = 1;
		*list = next->next;
		free(next->env_name);
		free(next->env_value);
		free(next);
		return (SUCCESS);
	}
	while (next && !ft_cmpstr(key, next->env_name))
	{
		prev = next;
		next = next->next;
	}
	if (!next)
		return (SUCCESS);
	prev->next = next->next;
	free(next);
	return (SUCCESS);
}

int	add_node_to(t_envs **head, t_envs **current)
{
	if (*head)
	{
		(*current)->next = *head;
		*head = *current;
	}	
	else
		*head = *current;
	return (SUCCESS);
}
