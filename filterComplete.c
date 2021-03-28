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
	if (ft_cmpstr("export", help->command))
	{
		ret = filter_check_envvar(help->txts);
		if (ret != SUCCESS)
			return PARSERROR;
	}
	return SUCCESS;
}

int filter_check_envvar(t_words *txts)
{
	int ret;
	int eq_pos;

	while (txts)
	{
		eq_pos = nlindex(txts->txt, '=');
		if (eq_pos -= 1){
			ret = check_envvar(txts->txt, eq_pos);
			if (ret != SUCCESS)
				return PARSERROR;
		}
		txts = txts->next;
	}
	return SUCCESS;
}


int modify_str(char **str)
{
	t_words *words = NULL; // just store words in str splited by " or  '
	int size;
	char *help;
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
		if (help[i] == '"' || help[i] == 39){ //  ""''ls''""
			c = help[i];
			start = i;
			while (help[++i] && help[i] != c)
				check = 1;
			size += (i - start);
			size -= 1;
		}
		else {
			start = i - 1;
			while (help[++i] && (help[i] != '"' && help[i] != 39))
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
		while (help->next)
			help = help->next;
		help->next = *word;
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


char *cleanWord(t_words *words, int size)
{
	char *tmp;
	int i;
	int j;

	i = -1;
	if (!(tmp = malloc(size + 1)))
		return 0;
	while (words)
	{
		j = -1;
		while (words->txt[++j])
			tmp[++i] = words->txt[j];
		words = words->next;
	}
	tmp[++i] = 0;
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
