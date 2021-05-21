#include "minishell.h"

int sub_export(t_fullvar **vars, char *line)
{
	char *key;
	int help;
	int ret;
	
	glob_vars.exitstatus = 0;	
	if (line == NULL)
	{
		export_print(*vars);
		return 0;
	}
	if (check_exvar(line) != SUCCESS)
		return glob_vars.exitstatus;
	key = get_key(line);
	ret = ft_strlen(key) - 1;
	glob_vars.envchanged = 1;
	h1_sub_export(key, vars, ret, line);
	return 0;
}

char *origin_var(char *var)
{
	int i;
	char *tmp;
	int j;

	j = var_length(var);
	i = -1;
	tmp = malloc(j + 1);
	j =  -1;
	while (var[++i])
	{
		if (is_special_in_double(var[i]))
			tmp[++j] = '\\';
		tmp[++j] = var[i];
	}
	tmp[++j] = 0;
	free(var);
	return tmp;
}

void export_print(t_fullvar *vars)
{
	t_envs *cur;
	t_words *keys;
	int found;

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

int check_exvar(char *line)
{
	int help;

	help = nlindex(line, '=');
	if (help == -1)
		help = ft_strlen(line);
	help = check_envvar(line, help);
	if (help != SUCCESS)
	{
		printf("bash: export: `%s': not a valid identifier\n", line);
		glob_vars.exitstatus = 1;
		return ENVERROR; // for error
	}
	return SUCCESS;
}

char *get_key(char *line)
{
	int eq_pos;

	eq_pos = nlindex(line, '=');
	if (eq_pos == -1)
		eq_pos = ft_strlen(line);
	return cutstring(line, 0, eq_pos);
}
