/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:32:39 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:32:40 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(char **args, t_fullvar **vars)
{
	int	i;
	int	ret;
	int	err;

	i = 0;
	ret = 0;
	err = 0;
	if (args == NULL || (args[0] && !args[1]))
	{
		g_vars.exitstatus = 0;
		return (0);
	}
	while (args[++i])
	{
		ret = unset_one_var(args[i], vars);
		if (ret)
			err = 1;
	}
	g_vars.exitstatus = err;
	return (err);
}

int	unset_one_var(char *key, t_fullvar **vars)
{
	t_envs	*var;
	int		help;

	g_vars.exitstatus = 0;
	if (check_unsetvar(key))
	{
		g_vars.exitstatus = 1;
		return (1);
	}
	var = get_env(&help, key, (*vars)->exenvs);
	if (help)
	{
		delete_env(&(*vars)->exenvs, key, &help);
		delete_exact_word(&(*vars)->allkeys, key);
		delete_exact_word(&(*vars)->filledvar, key);
		return (0);
	}
	if (ft_exist((*vars)->allkeys, key))
	{
		delete_exact_word(&(*vars)->allkeys, key);
		delete_exact_word(&(*vars)->filledvar, key);
	}
	return (0);
}

void	delete_exact_word(t_words **words, char *word)
{
	t_words	*prv;
	t_words	*next;

	next = *words;
	if (next && !ft_strcmp(word, next->txt))
	{
		*words = next->next;
		free(next->txt);
		free(next);
		return ;
	}
	while (next && ft_strcmp(word, next->txt))
	{
		prv = next;
		next = next->next;
	}
	if (next)
	{
		prv->next = next->next;
		free(next->txt);
		free(next);
	}
}

int	check_unsetvar(char *line)
{
	int	help;

	help = nlindex(line, '=');
	if (help == -1)
		help = ft_strlen(line);
	help = check_envvar(line, help);
	if (help != SUCCESS)
	{
		dup2(2, 1);
		printf("bash: unset: `%s': not a valid identifier\n", line);
		dup2(g_vars.fdout, 1);
		return (1);
	}
	help = ft_strlen(line) - 1;
	if (line[help] == '+')
	{
		dup2(2, 1);
		printf("bash: unset: `%s': not a valid identifier\n", line);
		dup2(g_vars.fdout, 1);
		return (1);
	}
	return (0);
}
