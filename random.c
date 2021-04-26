#include "minishell.h"


char *cutstring(char *str, int start, int last)
{
	char *tmp;
	int len;

	len = last - start + 1;
	if (!(tmp = malloc(len)))
		return 0;
	--start;
	len = -1;
	while (++start < last)
		tmp[++len] = str[start];
	tmp[++len] = 0;
	return tmp;
}

int istxt(char c)
{
	if (c != '<' && c != '>')
		return 1;
	return 0;
}

void free_dstr(char **str)
{
	int i;

	i = -1;
	while (str[++i])
		free(str[i]);
}

int ft_strcmp(char *s1, char *s2)
{
	int i;
	
	i = -1;
	while (s1[++i])
		if (s1[i] != s2[i])
			break ;
	return s1[i] - s2[i];
}

char* ft_itoa(int value)
{
    char *buffer;
	char *tmp;

	buffer = malloc(sizeof(char) * 4);
    int i = 0;
    while (value)
    {
        int r = value % 10;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
        value /= 10;
    }
    if (i == 0) {
        buffer[i++] = '0';
    }
    buffer[i] = '\0';
    tmp = ft_reverse(buffer);
	free(buffer);
	return tmp;
}

char *ft_reverse(char *line)
{
	int i;
	int j;
	char *tmp;

	j = ft_strlen(line);
	tmp = malloc(j + 1);
	i = -1;
	while (line[++i])
		tmp[i] = line[--j];
	tmp[i] = 0;
	return tmp;
}



int opvalid(char *str)
{
	if (ft_cmpstr(str, ">") || ft_cmpstr(str, "<") || ft_cmpstr(str, ">>"))
		return 1;
	return 0;
}


int ft_cmpstr(char *s1, char *s2){ // if the same returns 1 if not return 0

	int i;

	i = -1;
	while (s1[++i]){
		if (s1[i] != s2[i])
			return 0;
	}
	if (ft_strlen(s2 + i))
		return 0;
	return 1;

}

int reset_command(t_cmd **command)
{
	if (!(*command))
	{
		if (!(*command = malloc(sizeof(t_cmd))))
		{
			*command = NULL;
			return 0; // memory failure
		}
		(*command)->command = NULL;
		(*command)->txts = NULL;
		(*command)->files = NULL;
		(*command)->ops = NULL;
	}
	return  1;
}

char *ft_strdup(char *str)
{
	char *tmp;
	int len;

	len = ft_strlen(str) + 1;
	if (!(tmp = malloc(len)))
		return 0;
	len = -1;
	while (str[++len])
		tmp[len] = str[len];
	tmp[len] = 0;
	return tmp;
}

///  printing
void print_words(t_words *words, int level, char *name)
{
	if (words)
	{
		printspaces(level);
		printf("%s => %s|\n", name, words->txt);
		print_words(words->next, level + 1, name);
	}
}


void printspaces(int n)
{
	n *= 2;
	while (n-- > 0)
		write(1, " ", 1);
}

void print_pipes(t_words **pipes, int numofcmds)
{
	int i;
	
	i = -1;
	while (++i < numofcmds)
	{
		print_words(pipes[i], 0, "pipes");
		printf("--------------------------\n");
	}
}
