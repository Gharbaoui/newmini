#include "minishell.h"

char *get_full_expanded_line(t_cmd *cmd, t_envs **exenvs)
{
	char *cline;
	char *tline;
	char *line;
	t_words *txts;

	txts = cmd->txts;
	cline = expand_one_word(cmd->command, exenvs);
	while (txts)
	{
		tline = expand_one_word(txts->txt, exenvs);
		free(txts->txt);
		txts->txt = tline;
		txts = txts->next;
	}
	tline = get_line_from_words(cmd->txts);
	line = ft_strjoin(&cline, tline);
	free(tline);
	free_words(&cmd->txts);
	cmd->txts = NULL;
	cline = last_pars(line, &cmd->txts);
}

char *expand_one_word(char *str, t_envs **exenvs)
{
	t_words *words;
	char *tmp;

	words = split_by_qout(str);
	var_expan(words, exenvs);   /// variables should expand here
	tmp = get_line_from_words(words);
	free_words(&words);
	return tmp;
}

/////////// var expand   /////////
void var_expan(t_words *words, t_envs **exenvs)
{
	char *help;

	while (words)
	{
		help = get_words_expaned(words->txt, exenvs);
		free(words->txt);
		words->txt = help;
		words = words->next;
	}
}

char *get_words_expaned(char *str, t_envs **exenvs)
{
	t_envs *var;
	int totalsize;
	int i;
	char *key;
	int help;

	i = -1;
	totalsize = 0;
	if (str[0] == 39)
		return ft_strdup(str);
	return double_qout_part(str, exenvs);
}


char *double_qout_part(char *str, t_envs **exenvs)
{
	int help;
	char *tmp;
	int i;
	int j;

	i = -1;
	j = -1;
	help = get_len_double_qout(str, exenvs);
	tmp = malloc(help + 1);
	while (str[++i])
	{
		help = backslash(str, i);
		if (help % 2 == 0 && str[i] == '$')
			i+= fill_part_of_str(tmp, &j, str + i, exenvs);
		else
			tmp[++j] = str[i];
	}
	tmp[++j] = 0;
	return tmp;
}

int fill_part_of_str(char *tmp, int *index, char *str, t_envs **exenvs)
{
	char *key;
	int help;
	int j;
	int i;
	t_envs *var;

	j = -1;
	i = *index;
	get_var_name(str + 1, &key);
	var = get_env(&help, key, exenvs);
	if (help)
	{
		while (var->env_value[++j])
			tmp[++i] = var->env_value[j];
	}
	*index = i;
	j = ft_strlen(key);
	free(key);
	return j;
}

int get_len_double_qout(char *str, t_envs **exenvs)
{
	int i;
	t_envs *var;
	int totalsize;
	char *key;
	int help;

	totalsize = 0;
	i = -1;
	while(str[++i])
	{
		if (str[i] == '$')
		{
			help = backslash(str, i);
			if (help % 2 == 0)
			{
				get_var_name(str + i + 1, &key);
				i += ft_strlen(key);
				var = get_env(&help, key, exenvs);
				if (help)
					totalsize += ft_strlen(var->env_value) - (ft_strlen(key) + 1);
				free(key);
			}
		}
	}
	return i + totalsize;
}

/////////// var expand   /////////

///// *    from words to line    *////////
char *get_line_from_words(t_words *words)
{
	int size;
	char *tmp;
	int i;
	int j;

	i = -1;
	size = get_len_ofstrs_in_words(words);
	tmp = malloc(size + 1);
	while (words)
	{
		j = -1;
		while (words->txt[++j])
			tmp[++i] = words->txt[j];
		words = words->next;
	}
	tmp [++i] = 0;
	return tmp;
}

int get_len_ofstrs_in_words(t_words *words)
{
	int total;

	total = 0;
	while (words)
	{
		total += ft_strlen(words->txt);
		words = words->next;
	}
	return total;
}
///// *    from words to line    *////////


///// *    split by qout    *////////
t_words *split_by_qout(char *str)
{
	int i;
	int bst;
	char *help;
	t_words *w;

	w = NULL;
	i = -1;
	while (str[++i])
	{
		bst = backslash(str, i);
		if (bst % 2 == 0 && (str[i] == '"' || str[i] == 39))
		{
			if (str[i] == '"')
				help = get_quot_word(str, &i);
			else if (str[i] == 39)
				help = get_squto_word(str, &i);
		}else
			help = get_non_sdquot(str, &i);
		mk_and_add_to_words(&w, help);
		free(help);
	}
	return w;
}

char *get_quot_word(char *str, int *index)
{
	int i;
	int nextquot;

	i = *index;
	nextquot = index_of_dq(str, *index);
	*index = nextquot;
	return cutstring(str, i, nextquot + 1);
}

int  index_of_dq(char*str, int start)
{
	int help;


	while (str[++start])
	{
		if (str[start] == '"')
		{
			help = backslash(str, start);
			if (help % 2 == 0)
				return start;
		}
	}
	return -1999;
}

char *get_squto_word(char *str, int *index)
{
	int nextsq;
	int i;

	nextsq = nlindex(str + (*index + 1), 39) + (*index + 1);
	i = *index;
	*index = nextsq;
	return cutstring(str, i, nextsq + 1);
}

char *get_non_sdquot(char *str, int *index)
{
	int i;
	int help;
	char *tmp;
	i = *index - 1;
	while (str[++i])
	{
		help = backslash(str, i);
		if (help % 2 == 0 && (str[i] == '"' || str[i] == 39))
			break ;
	}
	tmp = cutstring(str, *index, i);
	*index = i - 1;
	return tmp;
}
///// *    split by qout    *////////

//////////////// last pars ////////////////

char *last_pars(char *line, t_words **txts)
{
	int help;
	int start;
	int i;

	i = -1;
	while (line[++i])
	{
		help = backslash(line, i);
		if (help % 2 == 0 && line[i] == '"')
		{
			start = i;
			i += get_next_dqpos(line + i);
		}else if (help % 2 == 0 && line[i] == 39)
		{
			start = i;
			i += get_next_sqpos(line + i);
		}else
		{
			start = i;
			i += get_next_nq(line + i);
		}
		mk_and_add_to_words(txts, cutstring(line, start, i + 1));
		i = skip_spaces(line, i);
	}

	//// i need after this to filter txts by removing quts and backslach usage
	// after store in AA the first txts->txt and move txts to txts->next and free(txts)
	// then return AA
}

int skip_spaces(char *line, int i)
{
	if (*line == 0)
		return i - 1;
	i++;
	while (line[i] == ' ')
		i++;
	return i - 1;
}

int get_next_dqpos(char *line)
{
	int dqfound;
	int i;
	int help;

	i = 0;
	dqfound = 0;
	while (line[++i])
	{
		if (line[i] == '"')
		{
			help = backslash(line, i);
			if (help % 2 == 0)
				dqfound = 1;
		}else if (dqfound && line[i] == ' ')
			break ;
	}
	return i - 1;
}	

int get_next_sqpos(char *line)
{
	int i;
	int sqfound;

	i = 0;
	sqfound = 0;
	while (line[++i])
	{
		if (line[i] == 39)
			sqfound = 1;
		else if (sqfound && line[i] == ' ')
			break ;
	}
	return i - 1;
}

int get_next_nq(char *line)
{
	int i;
	int help;

	i = 0;
	while (line[++i])
	{
		if (line[i] == ' ')
		{
			help = backslash(line, i);
			if (help % 2 == 0)
				break ;
		}
	}
	return i - 1;    ///// we are here
}


//////////////// last pars ////////////////
