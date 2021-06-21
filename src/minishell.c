/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:32:06 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:32:07 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	argc = 0;
	argv = NULL;
	tgetent(NULL, getenv("TERM"));
	bash_loop(env, 0, 0);
}

void	bash_loop(char **env, int ret, int i)
{
	char			c[2];
	t_fullvar		*variables;
	t_completecmd	*complete;

	signal(SIGINT, handl_sig);
	signal(SIGQUIT, handl_sig);
	variables = malloc(sizeof(t_fullvar));
	variables->exenvs = NULL;
	variables->filledvar = NULL;
	variables->allkeys = NULL;
	ret = fill_envtable(&variables, env);
	add_toenvtable(&variables, "?=0");
	g_vars.envchanged = 1;
	g_vars.line = ft_strdup("");
	g_vars.childruning = 0;
	g_vars.fdout = dup(1);
	g_vars.exenvs = &variables->exenvs;
	g_vars._pwd = NULL;
	update_pwd(variables->exenvs);
	level_of_bash(variables->exenvs);
	prompt();
	c[1] = 0;
	history_loop(&complete, &variables, c, i);
	close(g_vars.fdout);
}
