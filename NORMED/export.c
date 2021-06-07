#include "minishell.h"

int	sub_export(t_fullvar **vars, char *line)
{
	char	*key;
	char	*tmp;
	t_envs	*cuvar;
	int		help;
	int		ret;

	ret = help1_sub_export(vars, line, &key, &tmp);
	if (ret != -3)
		return (ret);
	help = help2_sub_export(vars, key, &tmp, &cuvar);
	if (help && nlindex(line, '=') != -1)
		help3_sub_export(line, key, &tmp, &cuvar);
	else
		add_toenvtable(vars, line);
	if (tmp)
		free(tmp);
	free(key);
	return (0);
}

void	export_print(t_fullvar *vars)
{
	t_envs	*cur;
	t_words	*keys;
	int		found;

	keys = vars->allkeys;
	sort_words(&keys);
	vars->allkeys = keys;
	while (keys)
	{
		cur = get_env(&found, keys->txt, vars->exenvs);
		if (!(keys->txt[0] == '_' && !keys->txt[1]))
		{
			if (found)
				printf("declare -x %s=\"%s\"\n", keys->txt, cur->env_value);
			else
				printf("declare -x %s\n", keys->txt);
		}
		keys = keys->next;
	}	
}

int	check_exvar(char *line)
{
	int	help;

	help = nlindex(line, '=');
	if (help == -1)
		help = ft_strlen(line);
	help = check_envvar(line, help);
	if (help != SUCCESS)
	{
		printf("bash: export: `%s': not a valid identifier\n", line);
		glob_vars.exitstatus = 1;
		return (ENVERROR);
	}
	return (SUCCESS);
}

char	*get_key(char *line)
{
	int	eq_pos;

	eq_pos = nlindex(line, '=');
	if (eq_pos == -1)
		eq_pos = ft_strlen(line);
	return (cutstring(line, 0, eq_pos));
}
