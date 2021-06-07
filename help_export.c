#include "minishell.h"

int	help1_sub_export(t_fullvar **vars, char *line, char **key, char **tmp)
{
	glob_vars.exitstatus = 0;
	if (line == NULL)
	{
		export_print(*vars);
		return (0);
	}
	if (check_exvar(line) != SUCCESS)
		return (glob_vars.exitstatus);
	*key = get_key(line);
	*tmp = NULL;
	glob_vars.envchanged = 1;
	return (-3);
}

int	help2_sub_export(t_fullvar **vars, char *key, char **tmp1, t_envs **cuvar1)
{
	int		ret;
	char	*tmp;
	int		help;
	t_envs	*cuvar;

	tmp = NULL;
	ret = ft_strlen(key) - 1;
	if (key[ret] == '+')
	{
		tmp = ft_strdup(key);
		tmp[ret] = 0;
		cuvar = get_env(&help, tmp, (*vars)->exenvs);
	}
	else
		cuvar = get_env(&help, key, (*vars)->exenvs);
	*tmp1 = tmp;
	*cuvar1 = cuvar;
	return (help);
}

void	help3_sub_export(char *line, char *key, char **tmp1, t_envs **cuvar)
{
	char	*tmp;

	tmp = *tmp1;
	if (tmp)
		free(tmp);
	tmp = split(line, 1, nlindex(line, '='));
	if (key[ft_strlen(key) - 1] == '+')
		(*cuvar)->env_value = ft_strjoin(&(*cuvar)->env_value, tmp);
	else
	{
		free((*cuvar)->env_value);
		(*cuvar)->env_value = ft_strdup(tmp);
	}
	*tmp1 = tmp;
}
