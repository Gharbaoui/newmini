#include "minishell.h"

int workon_line(char *line, t_completecmd **complete, int numofcmds, int help)
{
	int last, is_str;
	t_words *commands;
	t_workingcmds *wcmd;

	help = calcfirst(line, 92, &last, &is_str); /// checking for new line
	if (help != -2)
		return PARSERROR;
	rest(&wcmd, &commands);
	help = fill_commands(&commands, line);
	if (check_errors(help, &commands))
		return help;
	numofcmds = countnumberofcmds(commands); // count and it do some checking
	if (numofcmds <= 0) // parsing error -1 or 0 commands
	{
		free_words(&commands);
		return numofcmds;
	}
	help = fill_wcmd(&wcmd, commands, numofcmds);
	if (help != SUCCESS)
	{
		free_words(&commands);
		free_wcmd(&wcmd, numofcmds);
		return help;
	}
	help = fill_completecmd(complete, wcmd->cmds, 0);
	if (help != SUCCESS)
	{
		free_words(&commands);
		free_wcmd(&wcmd, numofcmds);
		free_comp(complete);
		return help;
	}
	help = filter_complete(complete);
	if (help != SUCCESS)
	{
		free_words(&commands);
		free_wcmd(&wcmd, numofcmds);
		free_comp(complete);
		return help;
	}
	print_completecmd(*complete);
	return SUCCESS;
}

// rest function 
void rest(t_workingcmds **wcmd, t_words **commands)
{
	*wcmd = NULL;
	*commands = NULL;
}
/// end rest function


// fill commands 
int fill_commands(t_words **commands, char *line)
{
	int is_str;
	int len;
	int last;

	len = calcfirst(line, ';', &last, &is_str);
	if (len == PARSERROR)
		return PARSERROR;
	if (!(*commands = malloc(sizeof(t_words))))
		return MEMERROR;
	rest_txt_next(&(*commands)->txt, &(*commands)->next);
	if (len >= 0)
	{
		if (fill_string_cmdtxt(commands, len, line) == MEMERROR)
			return 0;
		if ((is_str = fill_commands(&(*commands)->next, line + len + 1)) != SUCCESS)
			return is_str;
	}
	else
	{
		if (fill_string_cmdtxt(commands, last, line) == MEMERROR)
			return 0;
	}
	return SUCCESS;
}

int fill_string_cmdtxt(t_words **commands, int len, char *line)
{
	if (!((*commands)->txt = cutstring(line, 0, len)))
	{
		free(*commands);
		*commands = NULL;
		return MEMERROR;
	}
	return SUCCESS;
}

// rest string and struct (commands->txt, commands->next)
void rest_txt_next(char **str, t_words **next)
{
	*next = NULL;
	*str = NULL;
}

/// count number of commnds and do simple chrck
int countnumberofcmds(t_words *commands)
{
	int i;
	int check;
	t_words *last;

	i = -1;
	check = 0;
	while (commands)
	{
		last = commands;
		if (commands->next)
			if (simplecheck(commands->txt))
			{
				return PARSERROR;
			}
		commands = commands->next;
		i++;
	}
	help_short_count(last->txt, &i);
	return i;
}

void help_short_count(char *help, int *i)
{
	int check;
	int j;

	j = -1;	
	if (!help)
		return ;
	check = 0;
	while (help[++j])
	{
		if (help[j] != ' ')
		{
			check = 1;
			break;
		}
	}
	if (check)
		(*i)++;
}


/// fill_wcmd
int fill_wcmd(t_workingcmds **wcmd, t_words *commands, int numofcmds)
{
	int i;
	int ret;

    i = -1;
    t_words *pip;
	if ((help_short_fill_wcmd(wcmd, numofcmds, &pip)) != SUCCESS)
		return MEMERROR;
    while (++i < numofcmds)
    {
		ret = fill_pips(&pip, commands->txt);
        if (ret != SUCCESS){
			if (pip->txt)
				free(pip->txt);
			free(pip);
    		(*wcmd)->cmds[i] = NULL;
            return ret;
		}
        (*wcmd)->cmds[i] = pip;
        commands = commands->next;
    }
    (*wcmd)->cmds[i] = NULL;
	return SUCCESS;
}

int help_short_fill_wcmd(t_workingcmds **wcmd, int numofcmds, t_words **pipe)
{
	int i;

	i = -1;
    if (!(*wcmd = malloc(sizeof(t_workingcmds))))
        return MEMERROR; // memory failure
    if (!((*wcmd)->cmds = malloc(sizeof(t_words *) * (numofcmds + 1))))
    {
        free(wcmd);
        return MEMERROR;
    }
	*pipe = NULL;
	return SUCCESS;
}

int fill_pips(t_words **pip, char *line)
{
	int ret;
	int is_str;
    int len;
    int last;

    len = calcfirst(line, '|', &last, &is_str);
    if (!(*pip = malloc(sizeof(t_words))))
        return MEMERROR;
	if (!is_str || len == PARSERROR)
		return PARSERROR;
    if (len >= 0)
    {
        (*pip)->next = NULL;
		if (help_fill_pipes(pip, line, len) != SUCCESS)
			return MEMERROR;
        ret = fill_pips(&(*pip)->next, line + len + 1);
		if (ret != SUCCESS)
			return ret;
    }else{
		if (help_fill_pipes(pip, line, last) != SUCCESS)
			return MEMERROR;
    }
    return SUCCESS;
}

/// fill complete cmd structure
int fill_completecmd(t_completecmd **compcmd, t_words **pips, int pipindex)
{
	int ret;
	if (pips[pipindex])
	{
		if (!(*compcmd = malloc(sizeof(t_completecmd))))
			return 0;
		(*compcmd)->splcommand = NULL;
		ret = fill_pipcmd(&(*compcmd)->splcommand, pips[pipindex]);
		if (ret != SUCCESS)
		{
			free(*compcmd);
			*compcmd = NULL;
			return ret;
		}
		(*compcmd)->next = NULL;
		ret = fill_completecmd(&(*compcmd)->next, pips, pipindex + 1);
		if (ret != SUCCESS)
			return  ret;
	}
	return SUCCESS;	
}

int fill_pipcmd(t_pipcmd **pipcmd, t_words *pip)
{
	int ret;
	if (pip)
	{
		if (!(*pipcmd = malloc(sizeof(t_pipcmd))))
			return MEMERROR;
		(*pipcmd)->cmd = NULL;
		ret = splitlinetowords(pip->txt, &(*pipcmd)->cmd);
		if (ret != SUCCESS)
		{
			free(*pipcmd);
			*pipcmd = NULL;
			return ret;
		}
		(*pipcmd)->next = NULL;
		fill_pipcmd(&(*pipcmd)->next, pip->next);
	}
	return SUCCESS;
}

int splitlinetowords(char *str, t_cmd **command) // 2 success -1 parsing error 0 memory failure
{
	int help;

	help = 0;
	t_words *words;
	words = NULL;
	help = fill_words(&words, str);
	if (help !=  SUCCESS)
		return help; // means help =-1 or 0 memory failure or parsing error
	help = fill_cmdstruct(words, command);
	free_words(&words);
	if (help != SUCCESS)
		return help;
	return SUCCESS;
}


int fill_cmdstruct(t_words *words, t_cmd **command) // 0 memory failure -1 parsing error 1 success
{
	int ret;
	char *firstword;

	if (!reset_command(command))
		return 0;
	if (words)
	{
		firstword = words->txt;
		if (istxt(firstword[0]))
		{
			if (!(*command)->command){
				if (!((*command)->command = ft_strdup(firstword)))
					return MEMERROR; // memory failure
			}
			else
				if (!fill_command(command, firstword, 1)) // fill txt structure
					return MEMERROR; // memory failure
			ret = fill_cmdstruct(words->next, command);
			if (ret != SUCCESS)
				return ret;
		}
		else{
			if (opvalid(firstword))
			{
				if (!(fill_command(command, firstword, 3)))	// fill op
					return MEMERROR;
				words = words->next;
				if (words)
					firstword = words->txt;
				else
					return PARSERROR;
				if (istxt(firstword[0]))
				{
					if (!(fill_command(command, firstword, 2))) // fill files
						return MEMERROR;
					ret = fill_cmdstruct(words->next, command);
					if (ret != SUCCESS)
						return ret;
				}
				else
				   return PARSERROR; // parsing error because there is no file after > or < or >>
			}else
				return PARSERROR; // parsing error
		}
	}
	return SUCCESS;
}

int fill_command(t_cmd **command, char *str, int where)
{
	// if where 0 we will fill the command->command = str
	// if 1 fill str to txt to the list
	// if 2 ''''' files
	// if 3 ..... ops
	if (where == 3){
		if (!fill_cmd_objs(&(*command)->ops, str))
			return MEMERROR;
	}else if (where == 2)
	{
		if (!fill_cmd_objs(&(*command)->files, str))
			return MEMERROR;
	}else if (where == 1)
	{
		if (!fill_cmd_objs(&(*command)->txts, str))
			return MEMERROR;
	}
	return 1;
}

int fill_cmd_objs(t_words **txts, char *str)
{
	t_words *help;

	if (!(*txts))
	{
		if (!(*txts = malloc(sizeof(t_words))))
			return  MEMERROR; // memory failure
		(*txts)->next = NULL;
		if (!((*txts)->txt = ft_strdup(str)))
			return 0;
	}else{
		help  = *txts;
		while (help->next)
			help = help->next;
		if (!(help->next = malloc(sizeof(t_words))))
			return 0;
		help = help->next;
		if (!(help->txt = ft_strdup(str)))
			return 0;
		help->next = NULL;
	}
	return 1;
}

// shared functions 
int calcfirst(char *line, char c, int *last, int *is_str)
{
	int i;
	int dq;
	int  sq;

	*is_str = 0;
	if (help_short_calcfirst(&i, &dq, &sq, 1) && line[0] == '"') // that first condition just there to make func small in ines
		dq++;
	while (line[++i])
	{
		if (line[i] == '"' && sq % 2 == 0)
		{
			if (i > 0 && backslash(line, i) % 2 == 0)
				dq++;
		}
		else if (line[i] == 39 && dq % 2 == 0) // 39 asci for ' single
			sq++;
		if (line[i] == c && (dq % 2 == 0 && sq % 2 == 0))
			return i;
		if (line[i] != ' ' && line[i] !=  '|') /// 92 for '\'
			*is_str = 1;
	}
	*last = i;
	return help_short_calcfirst(&i, &dq, &sq, 0);
}


int help_short_calcfirst(int *i, int *dq, int *sq, int is_first)
{
	if (is_first == 1)
	{
		*i = -1;
		*dq = 0;
		*sq = 0;
	}else
	{	
		if (*dq % 2 != 0 || *sq % 2 != 0)
			return PARSERROR;
		return -2;
	}
	return 10; // this number has no meaning
}

int backslash(char *line, int index)
{
	int total;

	total = 0;
	while (--index > 0 && line[index] == 92)
			total++;
	return total;
}

int check_errors(int ern, t_words **words)
{
	if (ern != SUCCESS)
	{
		free_words(words);
		return ern;
	}
	return 0;
}

int simplecheck(char *line)
{
	int i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ')
			return 0;
	}
	if (i == ft_strlen(line))
		return 1;
	if (i == 1)
		return 1;
	return 0;
}


int help_fill_pipes(t_words **pip, char *line, int len)
{
	if (!((*pip)->txt = cutstring(line, 0, len)))
	{
		free(*pip);
		*pip = NULL;
		return MEMERROR;
	}
	(*pip)->next = NULL;
	return SUCCESS;
}
int fill_words(t_words **words, char *str)
{
	int start;
	int i;
	int check;

	i = -1;
	start = 0;
	while (*str && *str == ' ')
		str++;
	if (str[0])
	{
		if (!(*words = malloc(sizeof(t_words))))
			return MEMERROR;
		while (str[++i])
		{
			if (str[i] == '"' || str[i] == 39)
			{
				check = valditadsq(str + i);
				if (check < 0)
					return PARSERROR; // parsing error
				i += check;
			}
			else if (splitby(str, &i))
				break ;
		}
		if (!((*words)->txt = cutstring(str, start, i)))
		{
			free(*words);
			return MEMERROR;
		}
		(*words)->next = NULL;
		fill_words(&(*words)->next, str + i);
	}
	return SUCCESS; // completed
}

int valditadsq(char *str)
{
	int i;
	int last;

	i = 0;
	last = 0;
	if (str[0] == '"')
	{
		while (str[++i])
			if (i && str[i] == '"' && (last = backslash(str, i)) % 2 == 0)
				break;
		if (last % 2 != 0)
			return PARSERROR; // means we have open "
	}
	else if (str[0] == 39)
	{
		while (str[++i])
			if (str[i] == 39)
				break;
		if (i > 0 && str[i] != 39) // open  '
			return PARSERROR;
	}
	return i;
}

int  validchracter(char c)
{
	if (c == '>' || c == '<')
		return 0;
	return 1;
}

int splitby(char *str, int *index)
{
	int i;

	i = *index;
	if (str[i] == ' ')
		return 1;
	if (str[i] == '>' || str[i] == '<')
	{
		if (i  && validchracter(str[i - 1]))
			return 1;
		while (str[i] && (str[i] == '>' || str[i] == '<'))
			i++;
		*index = i;
		return 1;
	}
	return 0;
}

// all free here 
void free_wcmd(t_workingcmds **wcmd, int numofcmds)
{
	int i;

	i = -1;
	while (++i < numofcmds)
	{
		if ((*wcmd)->cmds[i])
			free_words(&(*wcmd)->cmds[i]);
	}
	if ((*wcmd)->cmds)
		free((*wcmd)->cmds);
	if (*wcmd)
		free(*wcmd);
}

void free_words(t_words **words)
{
	t_words *next;

	if (*words)
		next = (*words)->next;
	while (*words)
	{
		if ((*words)->txt)
		{
			free((*words)->txt);
		}
		free (*words);
		*words = next;
		if (*words)
			next = (*words)->next;
	}
}


void free_cmdstr(t_cmd **cmd)
{
	if (*cmd)
	{
		if ((*cmd)->command)
			free((*cmd)->command);
		if ((*cmd)->txts)
			free_words(&(*cmd)->txts);
		if ((*cmd)->files)
			free_words(&(*cmd)->files);
		if ((*cmd)->ops)
			free_words(&(*cmd)->ops);
		free(*cmd);
	}
}

void free_pipcmd(t_pipcmd **pipcmd)
{
	t_pipcmd *next;
	
	if (*pipcmd)
		next = (*pipcmd)->next;
	while (*pipcmd)
	{
		free_cmdstr(&(*pipcmd)->cmd);
		*pipcmd = next;
		if (*pipcmd)
			next = (*pipcmd)->next;
	}
}

void free_comp(t_completecmd **cmp)
{
	t_completecmd *next;
	
	if (*cmp)
		next = *cmp;
	while (*cmp)
	{
		free_pipcmd(&(*cmp)->splcommand);
		*cmp = next;
		if (*cmp)
			next = (*cmp)->next;
	}
}

