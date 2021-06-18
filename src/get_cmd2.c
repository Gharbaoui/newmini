/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 17:20:18 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 18:48:45 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	history_loop(t_completecmd **complete, t_fullvar **variables,
		char *c, int i)
{
	int		d;
	char	*tmp;

	tmp = NULL;
	i = 1;
	while (1)
	{
		d = get_char();
		c[0] = d;
		if (d >= 32 && d < 127)
			join_and_print(c, d);
		else if (d == ENTER)
			key_enter(complete, variables);
		else if (d == KEY_UP && g_vars.navigate)
			i = key_up(&tmp);
		else if (d == KEY_DOWN && i && g_vars.navigate2)
			i = key_down(&tmp);
		else if (d == KEY_ERASE)
			key_erase();
		else if (d == CTRL_RETURN)
			ctrl_return();
		else if (d == CTRL_D)
			ctrl_d();
	}
}

void	join_and_print(char *c, int d)
{
	g_vars.line = ft_strjoin(&g_vars.line, c);
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

void	ctrl_return(void)
{
	int		i;
	char	*s;

	i = -1;
	while (++i < ft_strlen(g_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
	}
	s = tgetstr("ce", NULL);
	write(1, s, ft_strlen(s));
	free(g_vars.line);
	g_vars.line = ft_strdup("");
}
