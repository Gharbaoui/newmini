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
    }    
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
    return SUCCESS;
}

int work_on_words(t_words **mod_words, t_words *words, t_envs **exenvs)
{
    t_words *cuw;
    int ret;
    if (words)
    {
        cuw = malloc(sizeof(t_words));
        ret = filter_string(&cuw, words->txt, exenvs);
        add_word_tofront(mod_words, &cuw);
        work_on_words(mod_words, words->next, exenvs);
    }
}

int filter_string(t_words **words, char *line, t_envs **exenvs)
{
    t_envs *cuenv;
    t_words *keys, *cuword;
    int ret, help;
    char c;
    char *key;
    int total;
    int envsize;
    char *backup;
    
    envsize = 0;
    total = 0;
    backup = line;
    if (line[0] != 39)
    {
        while (*line)
        {
            c = *line;
            if (c == 92 && is_special(line[1]))
            {
                total++;
                line++;
            }else if (c == '$')
            {
                line++;
                ret = get_var_name(line, &key);
                ret = ft_strlen(key);
                cuword = malloc(sizeof(t_words));
                cuword->txt = key;
                addtmptowords(&keys, &cuword);
                cuenv = get_env(&help, key, exenvs);
                total += ret + 1;
                if (help)  
                    envsize += ft_strlen(cuenv->env_value);
                line += ret - 1;
            }
            line++;
        }
        collect_strs(words, keys, backup, ft_strlen(backup) - total + envsize);
    }
    else{
        (*words)->txt = ft_strdup(line);
    }
    return SUCCESS;
}

int collect_strs(t_words **words, t_words *keys, char *line, int size)
{
    char *tmp;
    char *value;
    t_envs *var;
    int help;
    int i;
    int j;
    int k;

    i = -1;
    j = -1;
    tmp = malloc(size + 1);
    if (line[0] != 39)
    {
        while (line[++i])
        {
            if (line[i] == 92 && is_special(line[i + 1]))
            {
                i++;
            }else if (line[i] == '$')
            {
                i += ft_strlen(keys->txt);
                var = get_env(&help, keys->txt, ); //// left here need of t_envs exens variable not avialbel for now
                keys = keys->next;
            }
            tmp[++j] = line[i];
        }
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
}

void add_word_tofront(t_words **words, t_words **cuw)
{
    if (*words)
        (*cuw)->next = *words;
    else
        (*cuw)->next = NULL;
    *words = *cuw;
}





