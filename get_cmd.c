/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 11:20:47 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/16 14:35:05 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_return()
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

void	key_erase()
{
	char	*s;

	if (ft_strlen(glob_vars.line)) // to stop at the prompt if we delete all the line
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
		s = tgetstr("ce", NULL);
		write(1, s, ft_strlen(s));
		glob_vars.line[ft_strlen(glob_vars.line) - 1] = '\0';
	}
}

int key_down()
{
	char	*s;
	int		i;

	if (!glob_vars.navigate2->prev)
		return (0);
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
		write(1, glob_vars.navigate2->line, ft_strlen(glob_vars.navigate2->line));
		if (glob_vars.line)
			free(glob_vars.line);
		glob_vars.line = ft_strdup(glob_vars.navigate2->line);
		return (1);
	}
}

int key_up()
{
	char	*s;
	int 	i;

	i = -1;
	while (++i < ft_strlen(glob_vars.line))
	{
		s = tgetstr("le", NULL);
		write(1, s, ft_strlen(s));
	}
	i = 1;
	s = tgetstr("ce", NULL);
	write(1, s, ft_strlen(s));
	write(1, glob_vars.navigate->line, ft_strlen(glob_vars.navigate->line));
	if (glob_vars.line)
		free(glob_vars.line);
	glob_vars.line = ft_strdup(glob_vars.navigate->line);
	if (glob_vars.navigate != glob_vars.history)
		glob_vars.navigate2 = glob_vars.navigate;
	if (!glob_vars.navigate->next)
		i = 0;
	else
		glob_vars.navigate = glob_vars.navigate->next;
	return (i);
}