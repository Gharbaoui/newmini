/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 11:20:47 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/17 08:54:13 by mel-ghar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_return(void)
{
	int		i;
	char	*s;

	i = -1;
	while (++i < ft_strlen(glob_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
	}
	s = tgetstr("ce", NULL);
	write(1, s, ft_strlen(s));
	free(glob_vars.line);
	glob_vars.line = ft_strdup("");
}

void	key_erase(void)
{
	char	*s;

	if (ft_strlen(glob_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
		s = tgetstr("ce", NULL);
		write(1, s, ft_strlen(s));
		glob_vars.line[ft_strlen(glob_vars.line) - 1] = '\0';
	}
}

int	key_down(char *cur)
{
	char	*s;
	int		i;

	if (!glob_vars.navigate2->prev)
		return (h1_key_down(cur));
	else
	{
		i = -1;
		while (++i < ft_strlen(glob_vars.line))
		{
			s = tgetstr("le", NULL);
			write(1, s, ft_strlen(s));
		}
		s = tgetstr("ce", NULL);
		write(1, s, ft_strlen(s));
		glob_vars.navigate = glob_vars.navigate2;
		glob_vars.navigate2 = glob_vars.navigate2->prev;
		write(1, glob_vars.navigate2->line,
			ft_strlen(glob_vars.navigate2->line));
		if (glob_vars.line)
			free(glob_vars.line);
		glob_vars.line = ft_strdup(glob_vars.navigate2->line);
		return (1);
	}
}

int	h1_key_down(char *cur)
{
	int		i;
	char	*s;

	i = -1;
	while (++i < ft_strlen(glob_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
	}
	s = tgetstr("ce", NULL);
	write(1, s, ft_strlen(s));
	write(1, cur, ft_strlen(cur));
	if (glob_vars.line)
		free(glob_vars.line);
	glob_vars.line = ft_strdup(cur);
	return (0);
}

void	key_enter(t_completecmd **complete, t_fullvar **variables, char **cur)
{
	int	ret;

	if (check_history(glob_vars.line))
		push_to_history(&glob_vars.history, glob_vars.line);
	write(1, "\n", 1);
	ret = workon_line(glob_vars.line, complete, 0, 0);
	if (ret == PARSERROR)
	{
		ft_printf(1, "Parsing Error\n");
		free_comp(complete);
	}
	else
	{
		fullexcute(complete, variables);
		if (glob_vars.exit)
		{
			free(glob_vars.line);
			exit(glob_vars.exitstatus);
		}
	}
	free(*cur);
	*cur = ft_strdup("");
	key_enter2();
}
