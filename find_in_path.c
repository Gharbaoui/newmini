#include "minishell.h"

char	*get_command(char *cmd, char *PATH, int *prem, char *lcmd)
{
	struct stat	fst;
	char		*command;
	char		*b;
	char		*bcmd;
	int			ret;

	ret = h2_get_command(cmd, &b, &lcmd);
	if (ret == -1999)
		return (ft_strdup(""));
	if (ret)
		*prem = h1_get_command(ret, &command, cmd, b);
	else
	{
		*prem = 0;
		bcmd = ft_strjoin(&b, lcmd);
		command = get_using_path(bcmd, PATH, &fst);
		if (command)
			*prem = is_excutable(fst);
		else
			command = ft_strdup(lcmd);
		free (bcmd);
	}
	return (h3_get_command(lcmd, command));
}

char	*h3_get_command(char *lcmd, char *command)
{
	free(lcmd);
	return (command);
}

int	h1_get_command(int ret, char **command, char *cmd, char *b)
{
	int	prem;

	prem = 0;
	if (ret == 1)
	{
		*command = ft_strdup(cmd);
		prem = get_cmd_state(*command);
	}
	else
		*command = lower_str(cmd);
	free(b);
	return (prem);
}

int	h2_get_command(char *cmd, char **b, char **lcmd)
{
	*b = ft_strdup("/");
	*lcmd = lower_str(cmd);
	if (*cmd == '\0')
		return (-1999);
	return (is_comcmd(cmd, *lcmd));
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
