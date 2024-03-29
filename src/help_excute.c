/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_excute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:31:11 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:31:12 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	file_creation(char **files, char **ops)
{
	int	i;
	int	fd;

	i = -1;
	if (files)
	{
		while (files[++i])
		{
			if (check_name_file(files[i]))
				break ;
			if (ft_strcmp(ops[i], "<"))
			{
				fd = open (files[i], O_WRONLY | O_APPEND | O_CREAT, 0644);
				if (fd > 0)
					close(fd);
			}
			else
			{
				fd = open(files[i], O_RDONLY);
				if (fd < 0)
					break ;
				close(fd);
			}
		}
	}
}

void	alloc_pipes(int ***pipes, int count)
{
	int	i;

	i = -1;
	*pipes = malloc(sizeof(int *) * (count));
	while (++i < count)
	{
		(*pipes)[i] = malloc(sizeof(int) * 2);
		pipe((*pipes)[i]);
	}
}

int	get_num_subcmds(t_pipcommand *pcmd)
{
	int	i;

	i = 0;
	while (pcmd)
	{
		i++;
		pcmd = pcmd->next;
	}
	return (i);
}
