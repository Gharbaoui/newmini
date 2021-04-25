#include "minishell.h"

int ft_export(t_fullvar **vars, char *line)
{
	char *key, *tmp;
	t_envs *cuvar;
	int help;
	int ret;
	
/*	if (line == NULL){
		export_print((*vars)->exenvs);
		return SUCCESS;
	}*/
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
	if (help)
	{
		if (tmp)
			free(tmp);
		tmp = split(line, 1, nlindex(line, '='));
		if (key[ret] == '+')
		{
			cuvar->env_value = ft_strjoin(&cuvar->env_value, tmp);
		}else
		{
			if (tmp[0] != '\0')
			{
				free(cuvar->env_value);
				cuvar->env_value = ft_strdup(tmp); //// stops here
			}
		}
	}else{
		add_toenvtable(vars, line);
	}
	if (tmp)
		free(tmp);
	free(key);
}

int check_exvar(char *line)
{
	int help;

	help = check_envvar(line, nlindex(line, '='));
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
