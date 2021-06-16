/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 11:20:47 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/16 11:41:15 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

// void get_cmd(t_history **history, int d, char *s, int i)
// {
// 	char c;
// 	t_history *navigate = NULL;
// 	t_history *navigate2 = NULL;
	
// 	prompt();
// 	while (1)
// 	{
// 		d = get_char();
// 		c = d;
// 		i = 1;
// 		if (d >= 32 && d < 127)
// 		{
// 			glob_vars.line = ft_strjoin(&glob_vars.line, &c);
// 			write(1, &c, 1);
// 		}
// 		else if (d == ENTER)		
// 		{
// 			// TODO : check if line is empty , or contains one space , or previous cmd is the same as line.
// 			if (check_history(glob_vars.line))
// 				push_to_history(&history, glob_vars.line);
// 			break ;
// 			// write(1, "\n", 1);
			
		
			
// 			// prompt();
// 			// if (glob_vars.line)
// 			// 	free(glob_vars.line); 
// 			// glob_vars.line = ft_strdup("");
// 			// navigate = history;
// 			// navigate2 = history;
			
// 		}
// 		else if (d == KEY_UP && i && navigate)
// 		{
// 			i = -1;
// 			while (++i < ft_strlen(glob_vars.line))
// 			{
// 				s = tgetstr("le", NULL);
// 				write(1, s, ft_strlen(s));
// 			}
// 			i = 1;
// 			s = tgetstr("ce", NULL);
// 			write(1, s, ft_strlen(s));
// 			write(1, navigate->line, ft_strlen(navigate->line));
// 			glob_vars.line = navigate->line;
// 			if (navigate != history)
// 				navigate2 = navigate;
// 			if (!navigate->next)
// 				i = 0;
// 			else
// 				navigate = navigate->next;
// 		}
// 		else if (d == KEY_DOWN && i && navigate2)
// 		{
// 			if (!navigate2->prev)
// 				i = 0;
// 			else
// 			{
// 				i = -1;
// 				while (++i < ft_strlen(glob_vars.line))
// 				{
// 					s = tgetstr("le", NULL);
// 					write(1, s, ft_strlen(s));
// 				}
// 				s = tgetstr("ce", NULL);
// 				write(1, s, ft_strlen(s));
// 				navigate = navigate2;
// 				navigate2 = navigate2->prev;
// 				write(1, navigate2->line, ft_strlen(navigate2->line));
// 				glob_vars.line = navigate2->line;
// 			}
// 		}
// 		else if (d == KEY_ERASE)
// 		{
// 			if (ft_strlen(glob_vars.line)) // to stop at the prompt if we delete all the line
// 			{
// 				s = tgetstr("le", NULL);
// 				write(1, s, ft_strlen(s));
// 				s = tgetstr("ce", NULL);
// 				write(1, s, ft_strlen(s));
// 				glob_vars.line[ft_strlen(glob_vars.line) - 1] = '\0';
// 			}
// 		}
// 		else if (d == CTRL_RETURN)
// 		{
// 			i = -1;
// 			while (++i < ft_strlen(glob_vars.line))
// 			{
// 				s = tgetstr("le", NULL);
// 				write(1, s, ft_strlen(s));
// 			}
// 			s = tgetstr("ce", NULL);
// 			write(1, s, ft_strlen(s));
// 			free(glob_vars.line);
// 			glob_vars.line = ft_strdup("");
// 		}
		
// 	}
// }