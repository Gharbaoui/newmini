#include "minishell.h"

char	*get_fullstr(t_envs *var)
{
	char	*key;
	char	*value;
	char	*tmp;

	key = ft_strdup(var->env_name);
	value = var->env_value;
	tmp = ft_strjoin(&key, "=");
	tmp = ft_strjoin(&tmp, value);
	return (tmp);
}

char	**update_env_var(t_envs **exenvs)
{
	int		size;
	t_envs	*help;
	char	**full;
	int		i;
	int		j;

	size = get_hasht_size(exenvs);
	full = malloc(sizeof(char *) * (size + 1));
	i = -1;
	j = -1;
	while (++i < ENVSIZE)
	{
		help = exenvs[i];
		while (help)
		{
			full[++j] = get_fullstr(help);
			help = help->next;
		}
	}
	full[++j] = NULL;
	return (full);
}

int	number_of_vars(t_envs *exenv)
{
	int	total;

	total = 0;
	while (exenv)
	{
		total++;
		exenv = exenv->next;
	}
	return (total);
}

int	get_hasht_size(t_envs **exenvs)
{
	int	i;
	int	total;

	i = -1;
	total = 0;
	while (++i < ENVSIZE)
	{
		total += number_of_vars(exenvs[i]);
	}
	return (total);
}
