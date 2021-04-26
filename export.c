#include "minishell.h"

int sub_export(t_fullvar **vars, char *line)
{
	char *key, *tmp;
	t_envs *cuvar;
	int help;
	int ret;
	
	glob_vars.exitstatus = 0;	
	if (line == NULL){
		export_print(*vars);
		return SUCCESS;
	}
	if (check_exvar(line) != SUCCESS)
		return ENVERROR;
	key = get_key(line);
	ret = ft_strlen(key) - 1;
	tmp = NULL;
	if (key[ret] == '+'){
		tmp = ft_strdup(key);
		tmp[ret] = 0;
		cuvar = get_env(&help, tmp, (*vars)->exenvs);
	}else
		cuvar = get_env(&help, key, (*vars)->exenvs);
	if (help && nlindex(line, '=') != -1)
	{
		if (tmp)
			free(tmp);
		tmp = split(line, 1, nlindex(line, '='));
		if (key[ret] == '+')
		{
			cuvar->env_value = ft_strjoin(&cuvar->env_value, tmp);
		}else
		{
			free(cuvar->env_value);
			cuvar->env_value = ft_strdup(tmp); //// stops here
		}
	}else{
		add_toenvtable(vars, line);
	}
	if (tmp)
		free(tmp);
	free(key);
}

void export_print(t_fullvar *vars)
{
	t_envs *cur;
	t_words *keys;
	int found;

	keys = vars->allkeys;
	sort_words(&keys);
	while (keys)
	{
		cur = get_env(&found, keys->txt, vars->exenvs);
		if (!(keys->txt[0] == '_' && !keys->txt[1])){
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
