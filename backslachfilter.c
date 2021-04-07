#include "minishell.h"

int expand_current_command(t_completecmd **complet, t_fullvar *envs)
{
    static int cmdnum;
    t_completecmd *cucmd;
    int i;

    i = -1;
    cucmd = *complet;
    while (++i < cmdnum)
    {
        cucmd = cucmd->next;
        if (cucmd->next == NULL)
            cmdnum = 0;
    }
    expand_full_pipcmd(&cucmd->splcommand, envs->exenvs);
    cmdnum++;
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

    help = *cmd;
    if (help->command)
    {
        ret = backs_filter_str(&help->command, exenvs);
		split_command(cmd);
    }    
    return SUCCESS;
}

int split_command(t_cmd **cmd)
{
	
	return SUCCESS;
}


int backs_filter_str(char **str, t_envs **exenvs)
{
    t_words *words;
    t_words *mod_words;
    int ret;
    char *line;

    line = *str;
    words = NULL;
    mod_words = NULL;
    ret = local_words(&words, line);
    ret = work_on_words(&mod_words, words, exenvs, 0);
    free(*str);
    concatenate_words(mod_words, str);
	free_words(&words);
	free_words(&mod_words);
    return SUCCESS;
}

int work_on_words(t_words **mod_words, t_words *words, t_envs **exenvs, int order)
{
    t_words *cuw;

	cuw = NULL;
    int ret;
    if (words)
    {
        ret = filter_string(&cuw, words, exenvs, order);
        // next function will be here to decide if space in last
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


int local_words(t_words **words, char *line)
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
}
