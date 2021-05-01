#include "minishell.h"

char *get_command(char *cmd, char *PATH, int *prem, char *lcmd)
{
	struct stat fst;
	char *command;
	char *b;
	char *bcmd;
	int ret;

	b = malloc(2);
	b[0] = '/';
	b[1] = 0;
	lcmd = lower_str(cmd);
	if (cmd[0] == '\0')
		return ft_strdup("");
	ret = is_comcmd(cmd, lcmd); //// will return 1 if path provided or built in
	if (ret)
	{
		*prem = 0;
		if (ret == 1)
		{
			command = ft_strdup(cmd);
			*prem = get_cmd_state(command);
		}else
			command = ft_strdup(lcmd);
		free(b);
	}
	else{
		bcmd = ft_strjoin(&b, lcmd); 
		command = get_using_path(bcmd, PATH, &fst);
		if (command)
			*prem = is_excutable(fst);
		else{
			command = ft_strdup(lcmd); // in this situation means command not found
			*prem = 0;
		}
		free (bcmd);
	}
	free(lcmd);
	return command;	
}

int is_comcmd(char *cmd, char *lcmd) // if 1 means not look in PATH 0 look in PATH
{
	if (cmd[0] == '.' || cmd[0] == '/' || cmd[0] == '~')
		return 1;
	if (ft_cmpstr(lcmd, "echo") || ft_cmpstr(lcmd, "export") || ft_cmpstr(lcmd, "cd"))
		return 2;
	if (ft_cmpstr(lcmd, "exit") || ft_cmpstr(lcmd, "unset") || ft_cmpstr(lcmd, "env") || ft_cmpstr(lcmd, "pwd"))
		return 2;
	return 0;
}

int is_excutable(struct stat fst)
{
	if (fst.st_mode & S_IXUSR)
	{
		if (fst.st_mode  & S_IFDIR)
			return FOLDER;
		return 0;
	}
	return NEX;
}

int get_cmd_state(char *cmd)
{
	struct stat fst;

	if (stat(cmd, &fst) == 0)
		return is_excutable(fst);
	return 0;
}

char lower_char(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + ('a' - 'A');
	return c;
}

char *lower_str(char *str)
{
	char *tmp;
	int i;

	i = -1;
	tmp = malloc(ft_strlen(str) + 1);
	while (str[++i])
		tmp[i] = lower_char(str[i]);
	tmp[i] = 0;
	return tmp;
}


char *get_using_path(char *cmd, char *PATH, struct stat *fst)
{
	char *cpt;
	int index;
	int status;

	status = 1;
	while (*PATH)
	{
		cpt = get_one_path(PATH, &index);
		cpt = ft_strjoin(&cpt, cmd);
		if (stat(cpt, fst) == 0)
			return cpt;
		free(cpt);
		PATH += index;
		if(*PATH == ':')
			PATH++;
	}
	return NULL;
}

char *get_one_path(char *PATH, int *index)
{
	int cp; //colon position
	char *tmp;

	cp = nlindex(PATH, ':');
	if (cp == -1)
		cp = ft_strlen(PATH);
	tmp = cutstring(PATH, 0, cp);
	*index = cp;
	return tmp;
}

