#include "minishell.h"

int mk_and_add_to_words(t_words **words, char *line)
{
	t_words *cuw;

	cuw = malloc(sizeof(t_words));
	cuw->txt = ft_strdup(line);
	addtmptowords(words, &cuw);
	return SUCCESS;
}


int help_fill_tmp(char *tmp, char *value, t_strlen info, int is_first) // if status is one means we deal with spaces 0 do nothing
{
	t_words *words;
	int i;
	int start;
	int lastindex;
	int len;

	i = -1;
	i = 0;
	start = info.len;
	words = split_by_spaces(value, i);	
	//while (value[++i]){
	//	tmp[++start] = value[i];
	//}
	return start; 
}

/*int collect_strs(t_words **words, t_words *keys, t_envs **exenvs, t_strlen info)
{
	t_envs *cuvar, *nvar;
	t_words *cuw;
	t_strlen nextinfo;
    int i;
	int is_first;
	int status;
    char *line;
	int j;
	char *tmp;

	tmp = malloc(info.len + 1);
	i = -1;
	j = -1;
	is_first = 1;
    line = info.line;
	while (line[++i])
	{	if (line[i] == '$')
		{
			status = 0;
			i += ft_strlen(keys->txt);
			cuvar = get_env(&info.len, keys->txt, exenvs);
			if (info.len)
			{
				nextinfo.len = j;
				nextinfo.line = NULL;
				if (keys->next)
				{
					nvar = get_env(&info.len, keys->next->txt, exenvs);
					if (nvar)
						nextinfo.line = nvar->env_value;
				}
				j = help_fill_tmp(tmp, cuvar->env_value, nextinfo, is_first);
				is_first = 0;
			}
            keys = keys->next;
		}
		else if (line[i] == 92 && is_special(line[i + 1]))
			tmp[++j] = line[++i];
		else
			tmp[++j] = line[i];
	}
    tmp[++j] = 0;
    mk_and_add_to_words(words, tmp);
	free(tmp);
	return SUCCESS;
}*/

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

int concatenate_words(t_words *words, char **line) // i need to chage from "mohamed"opthrt'new' to mohamedopthrtnew remov quotes
{
    int len;
	t_strlen info;
    int i;
    char *tmp;

    len = get_lengthwords(words) + 1;
    tmp = malloc(len);
    i = -1;
	info.line = NULL;
    while (words)
    {
		info.len = i;
        i += help_fill_tmp(tmp, words->txt, info, 4); // should do nothing for the number 4
        words = words->next;
    }
    *line = tmp;
    return SUCCESS;
}

