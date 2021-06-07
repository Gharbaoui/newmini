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

int is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int is_number(char *num)
{
	if (*num == '-' || *num == '+')
		num++;
	while (*num)
	{
		if (!is_digit(*num))
			return (0);
		num++;
	}
	return (1);
}

void ft_printf(int count, ...)
{
	va_list list;
	int i;
	char *str;
	int j;

	i = -1;
	va_start(list, count);
	while (++i < count)
	{
		j = -1;
		str = va_arg(list, char *);
		while (str[++j])
			write(1, &(str[j]), 1);
	}
	va_end(list);
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

int		ft_atoi(char *str)
{
	long	result;
	int		np;
	long	max;

	np = 1;
	result = 0;
	max = 9223372036854775807;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
		np = (*str++ == '-') ? -1 : 1;
	while (ft_isdigit(*str))
	{
		if (result <= (max - (*str - '0')) / 10)
			result = (result * 10) + *str - '0';
		else
		{
			result = (np == -1) ? 0 : -1;
			break ;
		}
		str++;
	}
	return (result * np);
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

int	is_special(char c)
{
	if (c == 92 || c == '>' || c == '<' || c == '\'')
		return (1);
	if (c == '$' || c == '"' || c == '&')
		return (1);
	if (c == '|' || c == ']' || c == '[')
		return (1);
	if (c == '?' || c == '}' || c == '{')
		return (1);
	if (c == ';' || c == ':' || c == '/')
		return (1);
	if (c == '!' || c == '`' || c == '#')
		return (1);
	return (0);
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


void free_laststr(t_pipcommand **pcmd)
{
	t_pipcommand *help;

	help = *pcmd;
	while (help)
	{
		free_onecmd(&help->cmd);
		help = help->next;
	}
	*pcmd = NULL;
}

void free_pipes(int **pipes, int count)
{
	int i;

	i = -1;
	while (++i < count)
		free(pipes[i]);
	free(pipes);
}

void free_onecmd(t_onecmd *cmd)
{
	if (cmd)
	{
		if ((cmd)->cmd)
			free((cmd)->cmd);
		if ((cmd)->args)
		{
			free_dstr((cmd)->args);
			free(cmd->args);
		}
		if ((cmd)->files)
		{
			free_dstr((cmd)->files);
			free_dstr((cmd)->ops);
			free(cmd->files);
			free(cmd->ops);
		}
		free(cmd);
		cmd = NULL;
	}
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
