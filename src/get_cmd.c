/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 11:20:47 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 16:55:27 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	key_erase(void)
{
	char	*s;

	if (ft_strlen(g_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
		s = tgetstr("ce", NULL);
		write(1, s, ft_strlen(s));
		g_vars.line[ft_strlen(g_vars.line) - 1] = '\0';
	}
}

int	key_down(char **tmp)
{
	char	*s;
	int		i;

	if (!g_vars.navigate2->prev && *tmp)
	{
		i = -1;
		while (++i < ft_strlen(g_vars.line))
		{
			s = tgetstr("le", NULL);
			write(1, s, ft_strlen(s));
		}
		s = tgetstr("ce", NULL);
		write(1, s, ft_strlen(s));
		g_vars.navigate = g_vars.navigate2;
		if (g_vars.line)
			free(g_vars.line);
		g_vars.line = ft_strdup(*tmp);
		write(1, g_vars.line, ft_strlen(g_vars.line));
		free(*tmp);
		*tmp = NULL;
		return (0);
	}
	else
		return (key_down2());
}

int	key_down2(void)
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
	g_vars.navigate = g_vars.navigate2;
	g_vars.navigate2 = g_vars.navigate2->prev;
	write(1, g_vars.navigate2->line,
		ft_strlen(g_vars.navigate2->line));
	if (g_vars.line)
		free(g_vars.line);
	g_vars.line = ft_strdup(g_vars.navigate2->line);
	return (1);
}

int	key_up(char **tmp)
{
	char	*s;
	int		i;

	i = -1;
	while (++i < ft_strlen(g_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
	}
	i = 1;
	s = tgetstr("ce", NULL);
	write(1, s, ft_strlen(s));
	write(1, g_vars.navigate->line, ft_strlen(g_vars.navigate->line));
	if (*tmp == NULL)
		*tmp = ft_strdup(g_vars.line);
	if (g_vars.line)
		free(g_vars.line);
	g_vars.line = ft_strdup(g_vars.navigate->line);
	if (g_vars.navigate != g_vars.history)
		g_vars.navigate2 = g_vars.navigate;
	if (g_vars.navigate->next)
		g_vars.navigate = g_vars.navigate->next;
	return (1);
}

void	key_enter(t_completecmd **complete, t_fullvar **variables)
{
	int	ret;

	if (check_history(g_vars.line))
		push_to_history(&g_vars.history, g_vars.line);
	write(1, "\n", 1);
	ret = workon_line(g_vars.line, complete, 0, 0);
	if (ret == PARSERROR)
	{
		ft_printf(1, "Parsing Error\n");
		g_vars.exitstatus = 258;
		update_exit_status((*variables)->exenvs);
		free_comp(complete);
	}
	else
	{
		fullexcute(complete, variables);
		if (g_vars.exit)
		{
			free(g_vars.line);
			exit(g_vars.exitstatus);
		}
	}
	key_enter2();
}
