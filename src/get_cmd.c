/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 11:20:47 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/17 12:46:47 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	key_down(char *cur)
{
	char	*s;
	int		i;

	if (!g_vars.navigate2->prev)
		return (h1_key_down(cur));
	else
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
		g_vars.navigate2 = g_vars.navigate2->prev;
		write(1, g_vars.navigate2->line,
			ft_strlen(g_vars.navigate2->line));
		if (g_vars.line)
			free(g_vars.line);
		g_vars.line = ft_strdup(g_vars.navigate2->line);
		return (1);
	}
}

int	h1_key_down(char *cur)
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
	write(1, cur, ft_strlen(cur));
	if (g_vars.line)
		free(g_vars.line);
	g_vars.line = ft_strdup(cur);
	return (0);
}

void	key_enter(t_completecmd **complete, t_fullvar **variables, char **cur)
{
	int	ret;

	if (check_history(g_vars.line))
		push_to_history(&g_vars.history, g_vars.line);
	write(1, "\n", 1);
	ret = workon_line(g_vars.line, complete, 0, 0);
	if (ret == PARSERROR)
	{
		ft_printf(1, "Parsing Error\n");
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
	free(*cur);
	*cur = ft_strdup("");
	key_enter2();
}
