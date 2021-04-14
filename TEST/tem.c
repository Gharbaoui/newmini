#include "minishell.h"

int mk_and_add_to_words(t_words **words, char *line)
{
	t_words *cuw;

	cuw = malloc(sizeof(t_words));
	cuw->txt = ft_strdup(line);
	cuw->next = NULL;
	addtmptowords(words, &cuw);
	return SUCCESS;
}



int get_lengthwords(t_words *words)
{
    int total;

    total = 0;
    while (words){
        total += ft_strlen(words->txt);
        words = words->next;
    }
    return total;
}

int concatenate_words(t_words *words, char **line)
{
    int len;
    int i;
    int j;
    int lastindex;

    
    i = -1;
    len = get_lengthwords(words) + 1;
    *line = malloc(len);
    while (words)
    {
        j = -1;
        if (words->txt[0] == '"' || words->txt[0] == 39)
        {
            j = 0;
            lastindex = ft_strlen(words->txt) - 1;
            words->txt[lastindex] = 0;
        }
        while (words->txt[++j]){
            (*line)[++i] = words->txt[j];
        }
        words = words->next;
    }
    (*line)[++i] = 0;
    return SUCCESS;
}

int nonequt(char c)
{
	if (c != '"' && c != 39)
		return 1;
	return 0;
}

int first_one(t_words **help, char *line)
{
	if (nonequt(line[0]))
		get_words(line, help);
	else
		mk_and_add_to_words(help, line);
	return SUCCESS;
}

char *get_last_word(t_words *words)
{
	while (words && words->next)
		words = words->next;
	return words->txt;
}

int modify_prev(char *prv, char *cur)
{
	int index;

	index = ft_strlen(prv) - 1;
	if (prv[index] != ' ' && cur[0] != ' ')
		return 1;
	return 0;
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


int orgniz_mod_words(t_words *words, t_words **nw)
{
	char *tmp;
	char *back;
	t_words *head;
	int len;
	t_words *help;
	
	*nw = NULL;
	first_one(nw, words->txt);
	words = words->next;
	tmp = (*nw)->txt;
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
		add_words(nw, &help);
		words = words->next;
	}
	return SUCCESS;
}



int islastword(char *line)
{
	int i;

	i = -1;
	while (line[++i])
		if (line[i] != ' ')
			return 1; // not the last word
	return 0;
}

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

void free_one_word(t_words **word)
{
	if (*word)
	{
		if ((*word)->txt)
			free((*word)->txt);
		free (*word);
	}
}
