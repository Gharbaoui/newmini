/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h8_workonline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:30:52 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:30:53 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_commands(t_words **commands, char *line)
{
	int	is_str;
	int	len;
	int	last;

	len = calcfirst(line, ';', &last, &is_str);
	if (len == PARSERROR)
		return (PARSERROR);
	*commands = malloc(sizeof(t_words));
	if (!(*commands))
		return (MEMERROR);
	rest_txt_next(&(*commands)->txt, &(*commands)->next);
	if (len >= 0)
	{
		if (fill_string_cmdtxt(commands, len, line) == MEMERROR)
			return (0);
		is_str = fill_commands(&(*commands)->next, line + len + 1);
		if (is_str != SUCCESS)
			return (is_str);
	}
	else
		if (fill_string_cmdtxt(commands, last, line) == MEMERROR)
			return (0);
	return (SUCCESS);
}

int	fill_string_cmdtxt(t_words **commands, int len, char *line)
{
	(*commands)->txt = cutstring(line, 0, len);
	if (!(*commands)->txt)
	{
		free(*commands);
		*commands = NULL;
		return (MEMERROR);
	}
	return (SUCCESS);
}

int	fill_command(t_cmd **command, char *str, int where)
{
	if (where == 3)
	{
		if (!fill_cmd_objs(&(*command)->ops, str))
			return (MEMERROR);
	}
	else if (where == 2)
	{
		if (!fill_cmd_objs(&(*command)->files, str))
			return (MEMERROR);
	}
	else if (where == 1)
	{
		if (!fill_cmd_objs(&(*command)->txts, str))
			return (MEMERROR);
	}
	return (1);
}
