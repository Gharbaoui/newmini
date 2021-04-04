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
    char *help;
    t_words *words;
    int space;
    char *first;
    
    help = (*cmd)->command;
    if (help[0] != '"' && help[0] != 39)
    {
        local_words(&words, help);
        con_in_txts(cmd, words);
    }
	return SUCCESS;
}

int con_in_txts(t_cmd **cmd, t_words *words)
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
    ret = work_on_words(&mod_words, words, exenvs);
    free(*str);
    concatenate_words(mod_words, str);
	free_words(&words);
	free_words(&mod_words);
    return SUCCESS;
}

int work_on_words(t_words **mod_words, t_words *words, t_envs **exenvs)
{
    t_words *cuw;

	cuw = NULL;
    int ret;
    if (words)
    {
        ret = filter_string(&cuw, words->txt, exenvs);
        add_word_tofront(mod_words, &cuw);
        work_on_words(mod_words, words->next, exenvs);
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
        add_word_tofront(words, &word);
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





