/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_in_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:29 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:30 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command(char *cmd, char *PATH, int *prem, char *lcmd)
{
	char	*command;
	int		ret;

	if (cmd[0] == '\0')
		return (ft_strdup(""));
	lcmd = lower_str(cmd);
	ret = is_comcmd(cmd, lcmd);
	if (ret)
		*prem = help_get_command(cmd, ret, &command);
	else
		*prem = help2_get_command(lcmd, &command, PATH);
	free(lcmd);
	return (command);
}

int	is_comcmd(char *cmd, char *lcmd)
{
	if (cmd[0] == '.' || cmd[0] == '/' || cmd[0] == '~')
		return (1);
	if (ft_cmpstr(lcmd, "echo") || ft_cmpstr(lcmd, "export")
		|| ft_cmpstr(lcmd, "cd"))
		return (2);
	if (ft_cmpstr(lcmd, "exit") || ft_cmpstr(lcmd, "unset")
		|| ft_cmpstr(lcmd, "env") || ft_cmpstr(lcmd, "pwd"))
		return (2);
	return (0);
}

int	is_excutable(struct stat fst)
{
	if (fst.st_mode & S_IXUSR)
	{
		if (fst.st_mode & S_IFDIR)
			return (FOLDER);
		return (0);
	}
	return (NEX);
}

char	*get_using_path(char *cmd, char *PATH, struct stat *fst)
{
	char	*cpt;
	int		index;
	int		status;

	status = 1;
	while (*PATH)
	{
		cpt = get_one_path(PATH, &index);
		cpt = ft_strjoin(&cpt, cmd);
		if (stat(cpt, fst) == 0)
			return (cpt);
		free(cpt);
		PATH += index;
		if (*PATH == ':')
			PATH++;
	}
	return (NULL);
}

char	*get_one_path(char *PATH, int *index)
{
	int		cp;
	char	*tmp;

	cp = nlindex(PATH, ':');
	if (cp == -1)
		cp = ft_strlen(PATH);
	tmp = cutstring(PATH, 0, cp);
	*index = cp;
	return (tmp);
}
