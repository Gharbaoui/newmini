#include "minishell.h"

int fill_envtable(t_fullvar **fullvar, char **env)
{
	int ret;
	int help;

	help = rest_envt(&(*fullvar)->exenvs);
	if (help == MEMERROR)
		return MEMERROR;
	while (env[++help])
	{
		ret = add_toenvtable(fullvar, env[help]);
		if (ret != SUCCESS)
		{
			free_env(&(*fullvar)->exenvs);
			free ((*fullvar)->exenvs);
			(*fullvar)->exenvs = NULL;
			return MEMERROR;
		}
	}
	if (help == 0)
	{
		free ((*fullvar)->exenvs);
		return EMPTYENV;
	}
	return SUCCESS;
}


int add_toenvtable(t_fullvar **vars, char *line)
{
	int found;
	t_envs *currentEnv;
	char *help;
	int index;
	int ern;

	help = ft_strdup(line);
	help = origin_var(help);
	currentEnv = make_node_env(&ern, help, vars); // MEMERROR or
   	free(help);	
	if (currentEnv == NULL)
		return SUCCESS;
	if (ern != SUCCESS)
		return ern;
	index = hash_env_name(currentEnv->env_name);
	add_node_to(&(*vars)->exenvs[index], &currentEnv);
	return SUCCESS;
}

t_envs *make_node_env(int *ern, char *line, t_fullvar **vars)
{
	t_envs *currentEnv;
	int eq_pos;

	*ern = SUCCESS;
	if (!(currentEnv = malloc(sizeof(t_envs))))
	{
		*ern = MEMERROR;
		return NULL;
	}
	eq_pos = nlindex(line, '=');
	if (eq_pos != -1)
	{
		if (h1_make_n_env(vars, &currentEnv, eq_pos, line) == 0)
		{
			*ern = MEMERROR;
			return NULL;
		}
	}
	else
		h2_make_n_env(vars, &currentEnv, line);
	return currentEnv;
}

int ft_exist(t_words *words, char *str)
{
	while (words)
	{
		if (!ft_strcmp(words->txt, str))
			return 1;
		words = words->next;
	}
	return 0;
}

int check_envvar(char *line, int eq_pos)
{
	int i;

	i = 0;
	if (!ft_isalpha(line[0]) && line[0] != '_')
		return ENVERROR;
	while (++i < eq_pos - 1)
	{
		if (!ft_isalpha(line[i]) && !ft_isdigit(line[i]) && line[i] != '_')
			return ENVERROR;
	}
	if ((i < eq_pos) && !ft_isalpha(line[i]) &&
	!ft_isdigit(line[i]) && line[i] != '_' && line[i] != '+')
		return ENVERROR;
	return SUCCESS;
}	

int ft_isalpha(char c)
{
	if (c >= 65 && c <= 90)
		return 1;
	if (c >= 97 && c <= 122)
		return 1;
	return 0;
}

int ft_isdigit(char c)
{
	if (c >= 48 && c <= 57)
		return 1;
	return 0;
}

t_envs *get_env(int *found, char *env_name, t_envs **table)
{
	t_envs *current;
	int index;

	current = NULL;
	index = hash_env_name(env_name);
	if (table[index])
		current = look_inln(found, env_name, table[index]);
	else
		*found = 0;
	return current;
}

int add_to_words_str(t_words **hidden_var, char *line)
{
	t_words *current;

	if (!(current = malloc(sizeof(t_words))))
		return MEMERROR;
	if (!(current->txt = ft_strdup(line)))
	{
		free (current);
		return MEMERROR;
	}

	if (*hidden_var)
		current->next = *hidden_var;
	else 
		current->next = NULL;
	*hidden_var = current;
	return SUCCESS;
}

t_envs *look_inln(int *found, char *env_name, t_envs *lnenv)
{
	int ret;
	t_envs *cur;
	t_envs *next;

	*found = 0;
	cur = NULL;
	next = lnenv->next;
	while (lnenv)
	{
		ret = ft_cmpstr(env_name, lnenv->env_name);
		if (ret == 1)
		{
			*found = 1;
			break;
		}
		lnenv = next;
		if (lnenv)
			next = lnenv->next;
	}
	if (*found == 1)
		cur = lnenv;
	return cur;
}

int delete_env(t_envs ***table, char *env_name, int *founded)
{
	int index;

	index = hash_env_name(env_name);
	if ((*table)[index])
	{
		delete_exactfromln(&(*table)[index], env_name, founded);
	}
	return SUCCESS;
}

int delete_exactfromln(t_envs **list, char *key, int *found)
{
	t_envs *next;
	t_envs *prev;

	*found = 0;
	next = *list;
	if (next && ft_cmpstr(key, next->env_name))
	{
		*found = 1;
		*list = next->next;
		free(next->env_name);
		free(next->env_value);
		free(next);
		return SUCCESS;
	}
	while (next && !ft_cmpstr(key, next->env_name))
	{
		prev = next;
		next =  next->next;
	}
	if (!next)
		return SUCCESS;
	prev->next = next->next;
	free(next);
	return SUCCESS;
}

int add_node_to(t_envs **head, t_envs **current)
{
	if (*head)
	{
		(*current)->next = *head;
		*head = *current;
	}	
	else{
		*head = *current;
	}
	return SUCCESS;
}

int rest_envt(t_envs ***envtable)
{
	if (!((*envtable) = malloc(sizeof(t_envs *) * ENVSIZE)))
		return MEMERROR;
	int i;

	i = -1;
	while (++i < ENVSIZE)
	{
		(*envtable)[i] = NULL;
	}
	return -1;
}

int hash_env_name(char *env_name)
{
	int sum;
	int i;

	i = -1;
	sum = 0;
	while (env_name[++i])
		sum += env_name[i];
	sum *= env_name[i - 1];
	return sum % ENVSIZE;
}

void free_env(t_envs ***envtable)
{
	int i;

	i = -1;
	while (++i < ENVSIZE)
	{
		if ((*envtable)[i])
			free_envlist(&(*envtable)[i]);
	}
}

void free_envlist(t_envs **envlist)
{
	t_envs  *next;

	if (*envlist)
		next = (*envlist)->next;
	while (*envlist)
	{
		free_one_env(envlist);
		*envlist = next;
		if (*envlist)
			next = (*envlist)->next;
	}	
}

void free_one_env(t_envs **oneenv)
{
	if (*oneenv)
	{
		if ((*oneenv)->env_name)
			free((*oneenv)->env_name);
		if ((*oneenv)->env_value)
			free((*oneenv)->env_value);
		free(*oneenv);
	}
}


void print_tables(t_envs **table)
{
	int i;
	t_envs *help;

	i = -1;
	while (++i < ENVSIZE)
	{
		help = (table)[i];
		print_one_oft(help);
		printf("\033[0;32m");
		printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("\033[0m");
	}
}

void print_one_oft(t_envs *ln)
{
	t_envs *next;
	if (ln)
		next = ln->next;
	while (ln)
	{
		printf("|%s=%s|\n", ln->env_name, ln->env_value);
		printf("\033[0;33m");
		printf("----------------------------------------------------------\n");
		printf("\033[0m");
		ln = next;
		if (ln)
			next = ln->next;
	}
}
