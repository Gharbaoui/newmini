#include "minishell.h"

int	help_make_node_env(t_fullvar **vars, t_envs **curenv, char *line, int *ern)
{
	int		eq_pos;

	eq_pos = nlindex(line, '=');
	if (eq_pos != -1)
	{
		(*curenv)->env_name = split(line, 0, eq_pos);
		if (!(*curenv)->env_name)
			return (free_error(ern, (*curenv), 0));
		if ((*curenv)->env_name[eq_pos - 1] == '+')
			(*curenv)->env_name[eq_pos - 1] = '\0';
		(*curenv)->env_value = split(line, 1, eq_pos);
		if (!(*curenv)->env_value)
			return (free_error(ern, (*curenv), 1));
		(*curenv)->next = NULL;
		if ((*curenv)->env_name[0] != '?')
		{
			if (ft_exist((*vars)->allkeys, (*curenv)->env_name) == 0)
				add_to_words_str(&(*vars)->allkeys, (*curenv)->env_name);
			add_to_words_str(&(*vars)->filledvar, (*curenv)->env_name);
		}
	}
	else
		help2_make_node_env(vars, curenv, line);
	return (0);
}

void	help2_make_node_env(t_fullvar **vars, t_envs **cuenv, char *line)
{
	int	eq_pos;

	eq_pos = ft_strlen(line) - 1;
	if (line[eq_pos] == '+')
		line[eq_pos] = 0;
	if (ft_exist((*vars)->allkeys, line) == 0)
		add_to_words_str(&(*vars)->allkeys, line);
	free(*cuenv);
	*cuenv = NULL;
}

int	free_error(int *ern, t_envs *cuenv, int is_free)
{
	if (is_free)
		free(cuenv->env_name);
	free(cuenv);
	*ern = MEMERROR;
	return (1);
}

int	help_norm_make_node(t_envs **cuenv, t_envs **or, int *eq_pos, char *line)
{
	*eq_pos = nlindex(line, '=');
	*cuenv = *or;
	return (1);
}
