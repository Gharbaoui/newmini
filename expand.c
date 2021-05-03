#include "minishell.h"

t_pipcommand *expand_current_command(t_completecmd *cucmd, t_fullvar *envs)
{
	t_pipcommand *pc;

	if (cucmd){
    	expand_full_pipcmd(&cucmd->splcommand, envs->exenvs);
        return get_cmd_struct(cucmd->splcommand, envs->exenvs);
	}
    return NULL;
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

int expand_commandtxt(t_cmd **cmd, t_envs **exenvs)
{
	t_words *neww;
	t_words *head;
	neww = NULL;
	if ((*cmd)->command)
	{

		backs_filter_str(&(*cmd)->command, exenvs, &neww);
		free ((*cmd)->command);
		(*cmd)->command = neww->txt;  /// i will need strdup here
		head = neww;
		neww = neww->next;
		free(head);
		add_words(&neww, &(*cmd)->txts);
		(*cmd)->txts = neww;
	}
	return SUCCESS;
}

int expand_txts(t_words **txts, t_envs **exenvs)
{
	int i;
	int len;
	t_words *help;
	t_words **all;
	
	i = -1;
	help = *txts; //// look here
	all = expand_txtsh1(help, &len);
	while (help)
	{
		backs_filter_str(&(help->txt), exenvs, &all[++i]);
        if (all[i] == NULL)
        {
            i--;
            len--;
        }
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

int empty_var(t_words *words, t_envs **exenv)
{
    t_words *help;

	help = NULL;
    if (nonequt(words->txt[0])){
        work_on_words(&help, words, exenv, 0);
        if (help->txt[0] == 0)
            return 1;
    }
    return 0;
}

int backs_filter_str(char **str, t_envs **exenvs, t_words **newwords)
{
    t_words *words;
    t_words *mod_words;
	t_dollar *strdol;
    int ret;
    char *line;

    line = *str;
    words = NULL;
	strdol = NULL;
    mod_words = NULL;
    ret = local_words(&words, line, -1, &strdol);
	*newwords = NULL;
    if (line[0] == '$' && words->next == NULL && empty_var(words, exenvs))
        return SUCCESS;
	ret = work_on_words(&mod_words, words, exenvs, 0);
	ch_was_var(&mod_words);
	orgniz_mod_words(mod_words, newwords, strdol);
	free_words(&words);
	free_words(&mod_words);
	free_ints(strdol);
    return SUCCESS;
}

void free_ints(t_dollar *d)
{
	t_dollar *next;

	if (d)
		next = d->next;
	while (d)
	{	
		free(d);
		d = next;
	}
}

int local_words(t_words **words, char *line, int i, t_dollar **strdol)
{
	int index;
	int start;
	t_dollar *cdoll;
	t_words *word;
	int check;
	
	if (*line)
	{	
		while (line[++i])
		{
			cdoll = malloc(sizeof(t_dollar));
			cdoll->isd = 0;
			cdoll->next = NULL;
			start = i;
			word = local_wordsh1(&i, start, line);
			if (word->txt[0] == '$')
				cdoll->isd = 1;
			addstr_ints(strdol, &cdoll);
			addtmptowords(words, &word);
		}
	}else{
		cdoll = malloc(sizeof(t_dollar));
		cdoll->isd = 0;
		cdoll->next = NULL;
		word = malloc(sizeof(t_words));
		word->txt = malloc(1);
		word->txt[0] = 0;
		addstr_ints(strdol, &cdoll);
		addtmptowords(words, &word);
	}

	return SUCCESS;
}




t_words *local_wordsh1(int *index, int start,char *line)
{
	int i;
	int num;
	t_words *word;

	i = *index;
	word = malloc(sizeof(t_words));
	num = backslash(line, i);
	if ((line[i] == '"' && num % 2 == 0) || line[i] == 39)
	{
		i += valditadsq(line + i);
		word->txt = cutstring(line, start, i + 1);
	}else{
		while (line[++i]){
			num = backslash(line, i);
			if ((line[i] == '"' && num % 2 == 0) || line[i] == 39)
				break;
		}
		word->txt = cutstring(line, start, i);
		i--;
	}
	*index = i;
	return word;
}

int valid_space(char *line)
{
	int len;

	len = ft_strlen(line);
	while (--len >=0 && line[len])
		if (line[len] == 92)
			break ;
	if (line[len] == 92)
	{
		len++;
		len = backslash(line, len);
		if (len % 2 == 0)
			return 1;
		return 0;
	}
	return 1;
}

int work_on_words(t_words **mod_words, t_words *words, t_envs **exenvs, int order)
{
    t_words *cuw;
    int lastin;
    int ret;
	
	cuw = NULL;
    if (words)
    {
        ret = filter_string(&cuw, words, exenvs, order);
        if (!words->next && words->txt[0] != '"' && words->txt[0] != 39)
        {
            lastin = ft_strlen(cuw->txt) - 1;
            if (cuw->txt[lastin] == ' ' && valid_space(words->txt))
                cuw->txt[lastin] = 0;
        }
        addtmptowords(mod_words, &cuw);
        work_on_words(mod_words, words->next, exenvs, order + 1);
    }
	return SUCCESS;
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
		finleword = collect_strs(keys, exenvs, info, order); //// stops here
		free(info.line);
		free_words(&keys);
        addtmptowords(words, &finleword);
	}else{
		mk_and_add_to_words(words, w->txt);
	}
	return SUCCESS;
}


int get_var_name(char *line, char **key)
{
    int i;
    
    i = -1;
	if (line[0] == '?'){
		*key = ft_strdup("?");
		return SUCCESS;
	}
    while (line[++i])
        if (is_special(line[i]) || line[i] == ' ')
            break ;
    *key = cutstring(line, 0, i);
    return SUCCESS;
}

int is_special(char c)
{
    if (c == 92 || c == '>' || c == '<' || c == ' ')
        return 1;
    if (c == '$' || c == '"' || c == '&')
        return 1;
    if (c == '|' || c == ']' || c == '[')
        return 1;
    if (c == '?' || c == '}' || c == '{')
        return 1;
    if (c == ';' || c == ':' || c == '/')
        return 1;
    if (c == '!' || c == '`' || c == '#')
        return 1;
    return 0;
}


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

char *get_last_word(t_words *words)
{
	while (words && words->next)
		words = words->next;
	return words->txt;
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
		else if (line[i] == '$' && line[i + 1] && line[i + 1] != '/' && line[i + 1] != ' ')
		{
			varsize = loop_in_filter_stringh1(&i, line, keys, exenv);
			ret = ft_strlen(get_last_word(*keys));
			backtotal += ret + 1;
			i  += ret - 1;
		}
	}
	info.len = ft_strlen(line) - backtotal + varsize;
	info.line = ft_strdup(line);
	return info;
}

void collect_strs_h1(t_collstrs *vars, t_words **keys, t_envs **exenv, int order) //// returns status
{
	t_envs *cvar;
	int found;

	if (vars->line[vars->nums.i] == '$' && vars->line[vars->nums.i + 1] &&  vars->line[vars->nums.i + 1]  != '/' && vars->line[vars->nums.i + 1] != ' ')
	{
		if (vars->nums.i > 0)
			vars->nums.status = 1;
		vars->nums.i += ft_strlen((*keys)->txt);
		cvar = get_env(&found, (*keys)->txt, exenv);
		if (found)
		{
			if (vars->line[0] == '"')
				vars->nums.j = fill_all_var(vars->tmp, cvar->env_value, vars->nums.j);
			else if (order == 0 && vars->nums.status++ == 0)
				vars->nums.j = fill_first(vars->tmp, vars->nums.j, cvar->env_value);
			else
				vars->nums.j = fill_normal(vars->tmp, vars->nums.j, cvar->env_value);
		}
		(*keys) = (*keys)->next;
	}else if (vars->line[vars->nums.i] == 92 && is_special(vars->line[vars->nums.i + 1]))
		vars->tmp[++vars->nums.j] = vars->line[++vars->nums.i];
	else
		vars->tmp[++vars->nums.j] = vars->line[vars->nums.i];
}

void  help_fill_collstrs(t_strlen info, t_collstrs *vars)
{
	vars->line = info.line;
	vars->tmp = malloc(info.len + 1);
	vars->nums.i = -1;
	vars->nums.j = -1;
	vars->nums.status = 0;
}

t_words *collect_strs(t_words *keys, t_envs **exenv, t_strlen info, int order)
{
	t_collstrs vars;
	t_words *word = NULL;
	t_envs *cvar;
	
	help_fill_collstrs(info, &vars);
	while (info.line[++vars.nums.i])
	{
		collect_strs_h1(&vars, &keys, exenv, order);
	}
	vars.tmp[++vars.nums.j] = 0;
    mk_and_add_to_words(&word, vars.tmp);
    free(vars.tmp);
	return word;
}


int fill_all_var(char *tmp, char *value, int index)
{
	int i;

	i = -1;
	while (value[++i])
		tmp[++index] = value[i];
	return index;
}


int fill_first(char *tmp, int index, char *value)
{
    t_words *words;
	int ret;
    int i;

    words = split_by_spaces(value, 0);
    ret = fill_from_words(tmp, index, words);
	free_words(&words);
	return ret;
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
		words = first_case(line + i);
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


int mk_and_add_to_words(t_words **words, char *line)
{
	t_words *cuw;

	cuw = malloc(sizeof(t_words));
	cuw->txt = ft_strdup(line);
	cuw->next = NULL;
	addtmptowords(words, &cuw);
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


int orgniz_mod_words(t_words *words, t_words **nw, t_dollar *strdol)
{
	char *tmp;
	char *back;
	t_words *head;
	int len;
	t_words *help;
	
	*nw = NULL;
	if (strdol->isd){
		first_one(nw, words->txt);
		words = words->next;
		tmp = (*nw)->txt;
		strdol = strdol->next;
	}
	if (words == NULL && (tmp[0] == 39 || tmp[0] == '"'))
	{
		tmp++;
		tmp[ft_strlen(tmp) - 1] = 0;
		back = ft_strdup(tmp);
		free((*nw)->txt);
		(*nw)->txt = back;
	}
	while (words)
	{
		help = NULL;
		if (strdol->isd)
		{
			first_one(&help, words->txt);
			tmp = get_last_word(*nw);
			if (modify_prev(tmp, help->txt))
			{
				last_word(nw, help->txt);
				head = help;
				help = help->next;
				free (head->txt);
				free(head);
			}
		}else
		{
			fill_unchaged(&help, words->txt);	
		}
		strdol = strdol->next;
		add_words(nw, &help);
		words = words->next;
	}
	return SUCCESS;
}

void fill_unchaged(t_words **word, char *line)
{
	*word = malloc(sizeof(t_words));
	if (line[0] == '"' || line[0] == 39)
	{
		line++;
		line[ft_strlen(line) - 1] = 0;
	}
	(*word)->txt = ft_strdup(line);
	(*word)->next = NULL;
}

void add_words(t_words **orgin, t_words **forien)
{
	t_words *help;

	help = *orgin;
	while (help && help->next)
		help = help->next;
	if (help)
		help->next = *forien;
	else
		*orgin = *forien;
}


int first_one(t_words **help, char *line)
{
	if (nonequt(line[0]))
		get_words(line, help);
	else
		mk_and_add_to_words(help, line);
	return SUCCESS;
}
///////// long
int get_words(char *line, t_words **help)
{
	int i;
	int spindex;
	t_words *cuw;

	cuw = malloc(sizeof(t_words));
	if (line[0] == ' ')
		spindex = nlindex(line + 1, ' ') + 1;
	else
		spindex = nlindex(line, ' ');
	if (spindex == -1)
		spindex = ft_strlen(line);
	cuw->txt = cutstring(line, 0, spindex);
	addtmptowords(help, &cuw);
	if (line[spindex] == 0)
		line += spindex;
	else
		line += spindex + 1;
	while(*line)
	{
		spindex = nlindex(line, ' ');
		if (spindex == -1)
			spindex = ft_strlen(line);
		if (line[spindex] == ' ' && line[spindex + 1] == 0) // means last word
			spindex++;
		cuw = malloc(sizeof(t_words));
		cuw->txt = cutstring(line, 0, spindex);
		addtmptowords(help, &cuw);
		if (line[spindex] == 0)
			line += spindex;
		else
			line += spindex + 1;
	}
	return SUCCESS;
}

int last_word(t_words **nw, char *line){
	t_words *help;
	int index;
	int moved;
	char *s1;

	help = *nw;
	moved = 0;
	while (help && help->next)
		help = help->next;
	if (!nonequt(line[0]))
	{
		line++;
		line[ft_strlen(line) - 1] = 0;
	}
	if (line[0] == '"' || line[0] == 39)
	{
		line++;
		index = ft_strlen(line) - 1;
		line[index] = 0;
	}
	if (help->txt[0] == '"' || help->txt[0] == 39)
	{
		help->txt++;
		index = ft_strlen(help->txt) - 1;
		help->txt[index] = 0;
		moved = 1;
	}
	s1 = ft_strdup(help->txt);
	help->txt -= moved;
	free(help->txt);
	help->txt = ft_strjoin(&s1, line); // s1 has been freed inside 
	return SUCCESS;
}
/////// long


int modify_prev(char *prv, char *cur)
{
	int index;

	index = ft_strlen(prv) - 1;
	if (prv[index] != ' ' && cur[0] != ' ')
		return 1;
	return 0;
}


int nonequt(char c)
{
	if (c != '"' && c != 39)
		return 1;
	return 0;
}







