/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 17:20:18 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/17 12:46:47 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	history_loop(t_completecmd **complete, t_fullvar **variables,
		char *c, int i)
{
	char	*cur;
	int		d;

	cur = ft_strdup("");
	while (1)
	{
		d = get_char();
		c[0] = d;
		i = 1;
		if (d >= 32 && d < 127)
			h1_history_loop(&cur, d, c);
		else if (d == ENTER)
			key_enter(complete, variables, &cur);
		else if (d == KEY_UP && i && g_vars.navigate)
			i = key_up();
		else if (d == KEY_DOWN && i && g_vars.navigate2)
			i = key_down(cur);
		else if (d == KEY_ERASE)
			key_erase();
		else if (d == CTRL_RETURN)
			ctrl_return();
		else if (d == CTRL_D)
			ctrl_d();
	}
}

void	h1_history_loop(char **cur, int d, char *c)
{
	g_vars.line = ft_strjoin(&g_vars.line, c);
	free(*cur);
	*cur = ft_strdup(g_vars.line);
	write(1, &d, 1);
}

void	key_enter2(void)
{
	prompt();
	if (g_vars.line)
		free(g_vars.line);
	g_vars.line = ft_strdup("");
	g_vars.navigate = g_vars.history;
	g_vars.navigate2 = g_vars.history;
}

void	ctrl_d(void)
{
	if (*g_vars.line == 0)
	{
		write(1, "exit\n", 5);
		exit(g_vars.exitstatus);
	}
}
