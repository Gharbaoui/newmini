#include "minishell.h"

int expand_current_command(t_completecmd **complet, t_fullvar *envs)
{
    static int cmdnum;
    t_completecmd *cucmd;
    int i;

    i = -1;
    cucmd = *complet;
    while (cucmd &&  ++i < cmdnum)
    {
        cucmd = cucmd->next;
        if (cucmd->next == NULL)
            cmdnum = 0;
    }
	if (cucmd){
    	expand_full_pipcmd(&cucmd->splcommand, envs->exenvs);
		if (cucmd->next)
			cmdnum++;
	}else
		cmdnum = 0;
    return SUCCESS;
}

int expand_full_pipcmd(t_pipcmd **pipcmd, t_envs **exenvs)
{
    t_pipcmd *cupipcmd;

    cupipcmd = *pipcmd;
    while (cupipcmd)
    {
        expand_one_cmdstrct(&cupipcmd->cmd, exenvs);
        cupipcmd = cupipcmd->next;
    }
    return SUCCESS;
}

int expand_one_cmdstrct(t_cmd **cmd, t_envs **exenvs)
{   
    int ret;
    t_cmd *help;
	t_words *neww;
	t_words *head;


    help = *cmd;
	if (*cmd)
	{
		if ((*cmd)->txts)
			expand_txts(&(*cmd)->txts, exenvs);
		if ((*cmd)->command)
			expand_commandtxt(cmd, exenvs);
	}
    return SUCCESS;
}

int remove_spce_qu(t_words **words)
{
	t_words *help;


	help = *words;
	while (help)
	{
		if (!nonequt(help->txt[0]))
		{
			help->txt++;
			help->txt[ft_strlen(help->txt) - 1] = 0;
		}else if (help->txt[ft_strlen(help->txt) - 1] == ' ')
			help->txt[ft_strlen(help->txt) - 1] = 0;
		help = help->next;
	}
}


int expand_commandtxt(t_cmd **cmd, t_envs **exenvs)
{
	t_words *neww;
	t_words *head;
	neww = NULL;
	if ((*cmd)->command)
	{
		backs_filter_str(&(*cmd)->command, exenvs, &neww);
		free((*cmd)->command);
		(*cmd)->command = neww->txt;
		head = neww;
		neww = neww->next;
		free(head);
		add_words(&neww, &(*cmd)->txts);
		(*cmd)->txts = neww;
	}
	return SUCCESS;
}

/*int expand_txts(t_words **txts, t_envs **exenvs)
{
	int len;
	int i;
	t_words *help;
	t_words **all;
	
	help = *txts;
	len = 0;
	i = -1;
	while (help)
	{
		len++;
		help = help->next;
	}
	help = *txts;
	all = malloc(sizeof(t_words *) * len);
	while (help)
	{
		backs_filter_str(&(help->txt), exenvs, &all[++i]);
		help = help->next;
	}
	i = -1;
	free_words(txts);
	*txts = all[0];
	while (++i < len)
	{
		help = all[i];
		while (help &&  help->next)
			help = help->next;
		if (i + 1 < len)
			help->next = all[i + 1];
		else{
			help->next = NULL;
			break ;
		}
	}
	free(all);
	return SUCCESS;
}*/


//////
int expand_txts(t_words **txts, t_envs **exenvs)
{
	int i;
	int len;
	t_words *help;
	t_words **all;
	
	i = -1;
	help = *txts;
	all = expand_txtsh1(help, &len);
	while (help)
	{
		backs_filter_str(&(help->txt), exenvs, &all[++i]);
		help = help->next;
	}
	i = -1;
	free_words(txts);
	*txts = all[0];
	expand_txtsh2(len, all);
	return SUCCESS;
}



t_words **expand_txtsh1(t_words *words, int *l)
{
	int len;
	t_words **all;

	len = 0;
	while (words)
	{
		len++;
		words = words->next;
	}
	*l = len;
	all = malloc(sizeof(t_words *) * len);
	return all;
}

int expand_txtsh2(int len, t_words **all)
{
	int i;
	t_words *help;

	i = -1;
	while (++i < len)
	{
		help = all[i];
		while (help &&  help->next)
			help = help->next;
		if (i + 1 < len)
			help->next = all[i + 1];
		else{
			help->next = NULL;
			break ;
		}
	}
	free(all);
	return SUCCESS;
}
//////

int backs_filter_str(char **str, t_envs **exenvs, t_words **newwords)
{
    t_words *words;
    t_words *mod_words;
    int ret;
    char *line;

    line = *str;
    words = NULL;
    mod_words = NULL;
    ret = local_words(&words, line, -1);
    ret = work_on_words(&mod_words, words, exenvs, 0);
	orgniz_mod_words(mod_words, newwords);
	//remove_spce_qu(newwords);
	free_words(&words);
	free_words(&mod_words);
    return SUCCESS;
}

int work_on_words(t_words **mod_words, t_words *words, t_envs **exenvs, int order)
{
    t_words *cuw;
    int lastin;
	cuw = NULL;
    int ret;
    if (words)
    {
        ret = filter_string(&cuw, words, exenvs, order);
        if (!words->next && words->txt[0] != '"' && words->txt[0] != 39)
        {
            lastin = ft_strlen(cuw->txt) - 1;
            if (cuw->txt[lastin] == ' ')
                cuw->txt[lastin] = 0;
        }
        addtmptowords(mod_words, &cuw);
        work_on_words(mod_words, words->next, exenvs, order + 1);
    }
	return SUCCESS;
}




int get_var_name(char *line, char **key)
{
    int i;
    
    i = -1;
    while (line[++i])
        if (is_special(line[i]) || line[i] == ' ')
            break ;
    *key = cutstring(line, 0, i);
    return SUCCESS;
}

int is_special(char c)
{
    if (c == 92 || c == '>' || c == '<')
        return 1;
    if (c == '$' || c == '"' || c == '&')
        return 1;
    if (c == '|' || c == ']' || c == '[')
        return 1;
    if (c == '?' || c == '}' || c == '{')
        return 1;
    if (c == ';' || c == ':' || c == '/')
        return 1;
    if (c == '!' || c == '`' || c == '#' || c == 39)
        return 1;
    return 0;
}


/*int local_words(t_words **words, char *line)
{
    int i;
    int index;
    int start;
    t_words *word;
    int check;
    char c;

    i = -1;
    while (line[++i]) 
    {
        c = line[i];
        start = i;
        if (c == '"' || c == 39)
        {
            check = valditadsq(line + i); // could caus problems here not checking (check varible)
            i += check;
            word = malloc(sizeof(t_words));
            word->txt  = cutstring(line, start, i + 1); 
        }else{
            while (line[++i])
                if (line[i] == '"' || line[i] == 39)
                    break ;
            word = malloc(sizeof(t_words));
            word->txt  = cutstring(line, start, i);
            i--; 
        } 
        addtmptowords(words, &word);
    }
	return SUCCESS;
}
*/


int local_words(t_words **words, char *line, int i)
{
	int index;
	int start;
	t_words *word;
	int check;

	while (line[++i])
	{
		start = i;
		word = local_wordsh1(&i, start, line);
		addtmptowords(words, &word);
	}

	return SUCCESS;
}

t_words *local_wordsh1(int *index, int start,char *line)
{
	int i;
	t_words *word;

	i = *index;
	word = malloc(sizeof(t_words));
	if (line[i] == '"' || line[i] == 39)
	{
		i += valditadsq(line + i);
		word->txt = cutstring(line, start, i + 1);
	}else{
		while (line[++i])
			if (line[i] == '"' || line[i] == 39)
				break;
		word->txt = cutstring(line, start, i);
		i--;
	}
	*index = i;
	return word;
}









void add_word_tofront(t_words **words, t_words **cuw)
{
    if (*words)
        (*cuw)->next = *words;
    else
        (*cuw)->next = NULL;
    *words = *cuw;
}



t_words *split_by_spaces(char *line, int status)
{
	// status 0 means first we should not keep space at first word and we should let one at the end
	// status 1 means let at the first and at the end
	// status 2 means the last word let space in first and in last

	int i;
	
	i = 0;
	if (status == 0)
	{
		return first_case(line); // if space in last let one
	}else if (status == 1)
	{
		return second_case(line);  // let space in start and at the end
	}else if (status == 2){
		return third_case(line);
	}
	
	return NULL;
}

t_words *first_case(char *line)
{
	t_words *words;
	t_words *cuw;
	char *w;
	int next;

	words = NULL;
	while (*line == ' ')
		line++;
	while (*line)
	{
		w = get_word(line, &next);
		cuw = malloc(sizeof(t_words));
		cuw->txt = w;
		addtmptowords(&words, &cuw);
		line += next;
	}
	return words;
}

t_words *second_case(char *line)
{
	t_words *words;
	t_words *cuw = NULL;
	char *w;
	int *next;
	int space;
	int i;

	i = -1;
	
	words = NULL;
	if (*line == ' ')
		while (*(line + 1) == ' ')
			line++;
	space = nlindex(line + 1, ' ');
	if (space == -1)
		space = ft_strlen(line);
	w = malloc(space + 2);
	while (line[++i] && i < space + 1)
		w[i] = line[i];
	w[i] = 0;
	if (!(w[0] == ' ' && w[1] == '\0'))
	{
		words = first_case(line + i);
	}
    mk_and_add_to_words(&cuw, w);
    add_word_tofront(&words, &cuw);
	free(w);
	return words;
}

t_words  *third_case(char *line)
{
	t_words *words;
	t_words *head;
	int lastindex;


	words = second_case(line);
	head = words;
	while (words && words->next)
		words = words->next;
	if (words)
	{
		lastindex = ft_strlen(words->txt) - 1;
		if (words->txt[lastindex] == ' ')
			words->txt[lastindex] = 0;
	}
	return head;
}


char *get_word(char *line, int *next)
{
	char *tmp;
	int space;
	int j;
	int i;

	i = -1;
	j = -1;
	space = nlindex(line, ' ');
	if (space == -1)
		space = ft_strlen(line);
	tmp = malloc(space + 2); // 1 more maybe i will need to store one more space at the end
	if (*line == '\0')
		return NULL;
	while (line[++i] && i < space)
		tmp[++j] = line[i];

	while (line[i] && line[i] == ' ')
		++i;
	if (line[i - 1] == ' ' && line[i] == 0)
		tmp[++j] = ' ';
	tmp[++j] = 0;
	*next = i;
	return tmp;
}



int filter_string(t_words **words, t_words *w, t_envs **exenvs, int order)
{
	t_words *keys;
	t_words *finleword;
	t_strlen info;
	int i;

	keys = NULL;
	i = -1;
	keys = NULL;

	if (w->txt[0] != 39)
	{
		info = loop_in_filter_string(w->txt, exenvs, &keys);
		finleword = collect_strs(keys, exenvs, info, order);
		free(info.line);
		free_words(&keys);
        addtmptowords(words, &finleword);
	}else{
		mk_and_add_to_words(words, w->txt);
	}

	return SUCCESS;
}

int fill_all_var(char *tmp, char *value, int index)
{
	int i;

	i = -1;
	while (value[++i])
		tmp[++index] = value[i];
	return index;
}

int check_next(char *value, t_envs **exenv)
{
	char *key;
	int found;
	int i;
	t_envs *var;

	i = -1;
	if (*value == '"' || *value == 39)
		return 1;
	else if (value[0] != '$')
		return 1;
	get_var_name(value + 1, &key);
	var = get_env(&found, key, exenv);
	if (found)
	{
		while (var->env_value[i] && var->env_value[i] == ' ')
			i++;
		if (var->env_value[i] == '\0')
			return 0;
		else
			return 1;
	}
	return 0;
}

t_words *collect_strs(t_words *keys, t_envs **exenv, t_strlen info, int order)
{
	t_words *word = NULL;
	t_envs *cvar, *nvar;
	char *tmp;
    int status;
	char *keyvalue;
	int i;
	int j;

	i = -1;
    status = 0;
	j = -1;
	tmp = malloc(info.len + 1);
	while (info.line[++i])
	{
		if (info.line[i] == '$')
		{
			if (i > 0)
				status = 1;
			i += ft_strlen(keys->txt);
			cvar = get_env(&info.len, keys->txt, exenv);
			if (info.len)
			{
				if (info.line[0] == '"')
					j = fill_all_var(tmp, cvar->env_value, j);
				else if (order == 0 && status++ == 0)
                    j = fill_first(tmp, j, cvar->env_value);
                else
                    j = fill_normal(tmp, j, cvar->env_value);
			}
            keys = keys->next;
		}else if (info.line[i] == 92 && is_special(info.line[i + 1]))
			tmp[++j] = info.line[++i];
		else
			tmp[++j] = info.line[i];
	}
	tmp[++j] = 0;
    mk_and_add_to_words(&word, tmp);
    free(tmp);
	return word;
}

int fill_normal(char *tmp, int index, char *value)
{
    t_words *words;
    int status;
    
    status = 1;
    if(tmp[index] == ' ')
        status = 0;
    
    words = split_by_spaces(value, status);
    return fill_from_words(tmp, index, words);
}


int fill_first(char *tmp, int index, char *value)
{
    t_words *words;
    int i;

    words = split_by_spaces(value, 0);
    return fill_from_words(tmp, index, words);
}

int fill_from_words(char *tmp, int index, t_words *words)
{
    int i;
    
    i = -1;
    while (words)
    {
        i = -1;
        while (words->txt[++i])
            tmp[++index] = words->txt[i];
        words = words->next;
        if (words)
            tmp[++index] = ' ';
    }
    return index;
}

/*
t_strlen loop_in_filter_string(char *line, t_envs **exenv, t_words **keys)
{
	int backtotal;
	int varsize;
	t_words *cuw;
	int ret;
	t_envs *var;
	int i;
	t_strlen info;

	i = -1;
	backtotal = 0;
	varsize = 0;
	while (line[++i])
	{
		if (line[i] == 92 && is_special(line[i + 1]))
		{
			backtotal++;
			i++;
		}else if (line[i] == '$'){
			i++;
			cuw = malloc(sizeof(t_words));
			get_var_name(line + i, &cuw->txt);
			addtmptowords(keys, &cuw);
			var = get_env(&ret, cuw->txt, exenv);
			if (ret)
				varsize += ft_strlen(var->env_value);
			ret = ft_strlen(cuw->txt);
			backtotal += ret + 1;
			i += ret - 1;
		}
	}
	info.len = ft_strlen(line) - backtotal + varsize;
	info.line = ft_strdup(line);
	return info;
}*/




int loop_in_filter_stringh1(int *index, char *line, t_words **keys, t_envs **exenvs) /// return variable size
{
	t_words *cuw;
	int i;
	int varsize;
	int ret;
	t_envs *var;

	i = *index;
	i++;
	varsize = 0;
	cuw = malloc(sizeof(t_words));
	get_var_name(line + i, &cuw->txt);
	addtmptowords(keys, &cuw);
	var = get_env(&ret, cuw->txt, exenvs);
	if (ret)
		varsize = ft_strlen(var->env_value);
	*index = i;
	return varsize;
}

t_words *get_last_wordstruct(t_words *words)
{
	if (words)
	{
		while (words && words->next)
			words = words->next;
		return words;
	}
	return NULL;
}


t_strlen loop_in_filter_string(char *line, t_envs **exenv, t_words **keys)
{
	int i;
	int ret;
	int varsize;
	int backtotal;
	t_strlen info;

	i = -1;
	varsize = 0;
	backtotal = 0;
	while (line[++i])
	{
		if (line[i] == 92 && is_special(line[i + 1]) && ++i)
			backtotal++;
		else if (line[i] == '$')
		{
			varsize = loop_in_filter_stringh1(&i, line, keys, exenv);
			ret = ft_strlen(get_last_word(*keys)->txt);
			backtotal += ret + 1;
			i  += ret - 1;
		}
	}
	info.len = ft_strlen(line) - backtotal + varsize;
	info.line = ft_strdup(line);
	return info;
}


