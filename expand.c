#include "minishell.h"

int expand_current_command(t_completecmd **complet, t_fullvar *exenvs)
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
	free_words(&words);
	free_words(&mod_words);
    return SUCCESS;
}

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
            if (cuw->txt[lastin] == ' ')
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


