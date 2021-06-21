/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h2_gather_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:30:29 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:30:30 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		free_env_node(next);
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
	free_env_node(next);
	return (SUCCESS);
}

void	free_env_node(t_envs *n)
{
	if (n)
	{
		if (n->env_name)
			free(n->env_name);
		if (n->env_value)
			free(n->env_value);
		free(n);
	}
}

int	add_node_to(t_envs **head, t_envs **current)
{
	if (*head)
	{
		(*current)->next = *head;
		*head = *current;
	}	
	else
	{
		*head = *current;
	}
	return (SUCCESS);
}
