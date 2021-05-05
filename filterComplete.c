#include "minishell.h"

int filter_complete(t_completecmd **completecmd)
{	
	t_completecmd *help;
	int ret;

	help = *completecmd;
	
	while (help)
	{
		ret = filter_pipcmd1(&(help)->splcommand);
		if (ret != SUCCESS)
			return ret; // memory failure or parsing in export variables
		help = help->next;
	}
	return SUCCESS; // all good
}


int filter_pipcmd1(t_pipcmd **pipcmd)
{
	t_pipcmd *help;
	int ret;
	help = *pipcmd;

	while (help)
	{
		ret = filter_cmd(&(help)->cmd);
		if (ret != SUCCESS)
			return ret; // memory failure
		help = help->next;
	}
	return SUCCESS;
}


int filter_cmd(t_cmd **cmd)
{
	t_cmd *help;
	int check;
	int ret;
	check = 0;

	help = *cmd;
	if (!(modify_str(&help->command)))
			return MEMERROR; // memory problem
	if (!(modify_ln(&help->txts)))
		return MEMERROR; //memory failure
	if (!(modify_ln(&help->files)))
		return MEMERROR; // meemory failure
	return SUCCESS;
}

int filter_check_envvar(t_words *txts)
{
	int ret;
	int eq_pos;

	while (txts)
	{
		eq_pos = nlindex(txts->txt, '=');
		if (eq_pos == -1)
			eq_pos = ft_strlen(txts->txt);
		ret = check_envvar(txts->txt, eq_pos);
		if (ret != SUCCESS)
			return PARSERROR;
		txts = txts->next;
	}
	return SUCCESS;
}

int help_in_modstr(char *line, int i, char c)
{
	int num;
	if (c == 39){
		if (line[i] == 39)
			return 0;
	}
	else
	{
		num = backslash(line, i);
		if (line[i] == '"' && num % 2 == 0)
			return 0;
	}
	return 1;
}

int modify_str(char **str)
{
	t_words *words = NULL; // just store words in str splited by " or  '
	int size;
	char *help;
	int num;
	int i;
	char c;
	int check, start;

	help  = *str;
    if (!help)
        return 2;
	i = -1;
	size = 0;
	while (help[++i])
	{
		num = backslash(help, i);
		if (num % 2 == 0 && (help[i] == '"' || help[i] == 39)){ //  ""''ls''""
			c = help[i];
			start = i;
			while (help[++i] && help_in_modstr(help, i, c))
				check = 1;
			if (i - start > 1)
			{
				start--;
				i++;
			}
			size += (i - start);
			size -= 1;
		}
		else {
			start = i - 1; //// 
			while (help[++i] && (help[i] != '"' && help[i] != 39)) /// run hello \"
				check = 1;
			size += i - start;
			size -= 1;
			check = 3;
		}
		if (!(addtowords(&words, help, start, i)))
		{
			check = 5; //means malloc error
			break;
		}
		if (check == 3)
		{
			check = 0;
			i--;
		}
		else if (i - start > 1)
			i--;
	}
	free(*str);
	if (check == 5 || !(*str = cleanWord(words, size))) // maloc error
	{
		if (words)
			free_w(&words);
		return MEMERROR; // malloc error
	}
	if (words)
		free_w(&words);
	return SUCCESS; // all good
}

int addtowords(t_words **words, char *str, int start, int end){
	
	t_words *help;
	int len;
	char *tmp;
	int i;

	i = -1;
	len = end - start;
	if (!(help = malloc(sizeof(t_words))))
		return 0;
	if (!(tmp = malloc(len)))
	{
		free(help);
		return 0; // malloc error
	}
	while (++start < end)
		tmp[++i] = str[start];
	tmp[++i] = 0;
	help->txt = tmp;
	help->next = NULL;
	addtmptowords(words, &help);
	return 2; // all good
}



void addtmptowords(t_words **words, t_words **word)
{
	t_words *help;
	(*word)->next = NULL;
	if (!(*words))
		*words = *word;
	else{
		help = *words;
		while (help && help->next)
			help = help->next;
		help->next = *word;
	}	
}


void addstr_ints(t_dollar **head, t_dollar **new)
{
	t_dollar *help;
	(*new)->next = NULL;
	if (*head == NULL)
		*head = *new;
	else{
		help = *head;
		while (help && help->next)
			help = help->next;
		help->next = *new;
	}	
}

int modify_ln(t_words **words)
{
	t_words *help;
	help = *words;

	while (help)
	{
		if (!(modify_str(&help->txt)))
			return MEMERROR; //memory failure
		help = help->next;
	}
	return SUCCESS;
}

int theres_empty(t_words *words)
{
    while (words)
    {   
        if (words->txt[0] == '\0')
            return 1;
        words = words->next;
    }
    return 0;
}

char *cleanWord(t_words *words, int size)
{
    t_words *head;
	char *tmp;
    char *ptr;
    //char *help;
	int i;
	int j;

	i = -1;
    head = words;
	if (!(tmp = malloc(size + 1)) /*|| !(help = ft_strdup("''"))*/)
		return 0;
	while (words)
	{
		j = -1;
		while (words->txt[++j])
			tmp[++i] = words->txt[j];
		words = words->next;
	}
	tmp[++i] = 0;
    /*if (tmp[0] != 0 && theres_empty(head))
    {
        ptr = tmp;
        tmp = ft_strjoin(&help, tmp);
        free(ptr);
    }else
		free(help);*/
	return tmp;
}


void free_w(t_words **words)
{
	t_words *next;
	next = (*words)->next;

	while (*words)
	{
		if ((*words)->txt)
			free((*words)->txt);
		free(*words);
		*words = next;
		if (*words)
			next = (*words)->next;
	}	
}
