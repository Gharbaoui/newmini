#include "minishell.h"

int	ft_isalpha(char c)
{
	if (c >= 65 && c <= 90)
		return (1);
	if (c >= 97 && c <= 122)
		return (1);
	return (0);
}

int	ft_isdigit(char c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

t_envs	*get_env(int *found, char *env_name, t_envs **table)
{
	t_envs	*current;
	int		index;

	current = NULL;
	index = hash_env_name(env_name);
	if (table[index])
		current = look_inln(found, env_name, table[index]);
	else
		*found = 0;
	return (current);
}

int	add_to_words_str(t_words **hidden_var, char *line)
{
	t_words	*current;

	current = malloc(sizeof(t_words));
	if (!current)
		return (MEMERROR);
	current->txt = ft_strdup(line);
	if (!current->txt)
	{
		free (current);
		return (MEMERROR);
	}
	if (*hidden_var)
		current->next = *hidden_var;
	else
		current->next = NULL;
	*hidden_var = current;
	return (SUCCESS);
}
