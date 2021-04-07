#include "minishell.h"

int mk_and_add_to_words(t_words **words, char *line)
{
	t_words *cuw;

	cuw = malloc(sizeof(t_words));
	cuw->txt = ft_strdup(line);
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

