#include "minishell.h"

void h1_sub_export(char *key, t_fullvar **vars, int ret, char *line)
{
	char *tmp;
	int help;
	t_envs *cuvar;
	
	tmp = NULL;
	if (key[ret] == '+')
	{
		tmp = ft_strdup(key);
		tmp[ret] = 0;
		cuvar = get_env(&help, tmp, (*vars)->exenvs);
	}
	else
		cuvar = get_env(&help, key, (*vars)->exenvs);
	if (help && nlindex(line, '=') != -1)
		h_h1_sub_export(&tmp, line, &cuvar, key);
	else
		add_toenvtable(vars, line);
	if (tmp)
		free(tmp);
	free(key);
}


void h_h1_sub_export(char **tmp, char *line, t_envs **cuvar, char *key)
{
	int ret;

	ret = ft_strlen(key) - 1;
	if (*tmp)
		free(*tmp);
	*tmp = split(line, 1, nlindex(line, '='));
	*tmp = origin_var(*tmp);
	if (key[ret] == '+')
		(*cuvar)->env_value = ft_strjoin(&(*cuvar)->env_value, *tmp);
	else
	{
		free((*cuvar)->env_value);
		(*cuvar)->env_value = ft_strdup(*tmp); //// stops here
	}
}
