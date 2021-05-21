#include "minishell.h"

int h1_make_n_env(t_fullvar **vars, t_envs **cur_env, int eq_pos, char *line)
{
	(*cur_env)->env_name = split(line, 0, eq_pos);
	if 	((*cur_env)->env_name[eq_pos - 1] == '+')
		(*cur_env)->env_name[eq_pos - 1] = '\0';
	if (!(*cur_env)->env_name)
	{
		free ((*cur_env)->env_name);
		free((*cur_env));
		return 0;
	}
	if (!((*cur_env)->env_value = split(line, 1, eq_pos)))
	{
		free ((*cur_env)->env_name);
		free((*cur_env));
		return 0;
	}
	(*cur_env)->next = NULL;
	if ((*cur_env)->env_name[0] != '?'){
		if (ft_exist((*vars)->allkeys, (*cur_env)->env_name) == 0)
			add_to_words_str(&(*vars)->allkeys, (*cur_env)->env_name);
		add_to_words_str(&(*vars)->filledvar, (*cur_env)->env_name);
	}
	return 1;
}

void h2_make_n_env(t_fullvar **vars, t_envs **cur_env, char *line)
{
	int eq_pos;

	eq_pos = ft_strlen(line) - 1;
	if (line[eq_pos] == '+')
		line[eq_pos] = 0;
	if (ft_exist((*vars)->allkeys, line) == 0)
		add_to_words_str(&(*vars)->allkeys, line);
	free((*cur_env));
	(*cur_env) = NULL;
}
