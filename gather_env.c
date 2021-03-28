#include "minishell.h"

int fill_envtable(t_fullvar **fullvar, char **env)
{
	int ret;
	int help;

	help = rest_envt(&(*fullvar)->exenvs);
	if (help == MEMERROR)
		return MEMERROR;
	help = -1;
	while (env[++help])
	{
		ret = add_toenvtable(&(*fullvar)->exenvs, env[help], &(*fullvar)->allkeys);
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


int add_toenvtable(t_envs ***envtable, char *line, t_words **allkeys)
{
	int found;
	t_envs *currentEnv;
	int index;
	int ern;
	
	currentEnv = make_node_env(&ern, line, allkeys); // MEMERROR or 
	if (ern != SUCCESS)
		return ern;
	index = hash_env_name(currentEnv->env_name);
	add_node_to(&(*envtable)[index], &currentEnv);
	return SUCCESS;
}

t_envs *make_node_env(int *ern, char *line, t_words **allkeys)
{
	t_envs *currentEnv;
	int eq_pos;

	// i need to chevck first if line is valid as env variable
	*ern = SUCCESS;
	if (!(currentEnv = malloc(sizeof(t_envs))))
	{
		*ern = MEMERROR;
		return NULL;
	}
	eq_pos = nlindex(line, '=');
	if (eq_pos != -1)
	{
		currentEnv->env_name = split(line, 0, eq_pos);
		if (!currentEnv->env_name)
		{
			*ern = MEMERROR;
			free (currentEnv->env_name);
			free(currentEnv);
			return NULL;
		}
		if (!(currentEnv->env_value = split(line, 1, eq_pos)))
		{
			free (currentEnv->env_name);
			free(currentEnv);
			*ern = MEMERROR;
			return NULL;
		}
		currentEnv->next = NULL;
	}
	return currentEnv;
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
	if ((i < eq_pos) && !ft_isalpha(line[i]) && !ft_isdigit(line[i]) && line[i] != '_' && line[i] != '+')
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
	{
		current = look_inln(found, env_name, table[index]); // if found 0 means that there's no key
	}
	else{
		*found = 0;
	}
	return current;
}

int add_envvar_to_table(t_envs **extable, char *line, t_words **free_envvar)
{
	int is_plus;
	int help; // plays also as index of '=' and length of kry variable 
	char *keyvalue; // sometimes plays as key and sometimes plays as value
	int index;
	t_envs *ptr;
	
	index = nlindex(line, '=');
	is_plus = 0;
	if (index != -1)
	{
		keyvalue = split(line, 0, index);
		if (keyvalue[index  - 1] == '+')
		{
			keyvalue[index - 1] = 0;
			is_plus = 1;
		}
		ptr = get_env(&help, keyvalue, extable);
		free(keyvalue);
	}else{
		ptr = get_env(&help, line, extable);
		if (help == 0)
		{
			help = add_tofree_envvar(free_envvar, line);
			if (help != SUCCESS)
				return help;
		}
		return SUCCESS;
	}
	if (help) // here check if you find already variable stored
	{
		keyvalue = split(line, 1, index);
		if (is_plus){
			ptr->env_value = ft_strjoin(&ptr->env_value, keyvalue);
			free(keyvalue);
		}else{
			free (ptr->env_value);
			ptr->env_value = keyvalue;
		}
	}else{
		//help = add_toenvtable(&extable, line);
		if (help != SUCCESS)
			return help;
	}
	return SUCCESS;
}

int add_tofree_envvar(t_words **hidden_var, char *line)
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
	{
		cur = lnenv;
	}
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
		free(next);
		return SUCCESS;
	}
	
	while (next && !ft_cmpstr(key, next->env_name))
	{
		prev = next;
		next =  next->next;
	}
	if (!next)
	{
		return SUCCESS;
	}

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
	return SUCCESS;
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
