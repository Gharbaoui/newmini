/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 11:15:53 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/16 13:10:28 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_char(void)
{
	char			c;
	int				total;
	struct termios	term;
	struct termios	init;

	tcgetattr(0, &term);
	tcgetattr(0, &init);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
	total = 0;
	while (read(0, &c, 1) <= 0)
		;
	total += c;
	while (read(0, &c, 1) > 0)
		total += c;
	tcsetattr(0, TCSANOW, &init);
	return (total);
}

int prompt()
{
	write(1, "\033[1;32m", ft_strlen("\033[1;32m"));
	write(1, "MINISHELL -> ", 13);
	write(1, "\x1b[0m", 4);
	return (1);
}

int	push_to_history(t_history **h, char *line)
{
	t_history	*new;

	new = malloc(sizeof(t_history));
	new->line = ft_strdup(line);
	new->prev = NULL;
	if (*h == NULL)
	{
		new->next = NULL;
		*h = new;
		return (1);
	}
	new->next = *h;
	(*h)->prev = new;
	*h = new;
	return (0);
}
int check_history(char *line)
{
	if (ft_strcmp(line, "") == 0)
	{
		// write(1, "HOLA\n", 5);
		return 0;
	}
	return (1);
}