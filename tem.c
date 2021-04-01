#include "minishell.h"

int mk_and_add_to_words(t_words **words, char *line)
{
	t_words *cuw;

	cuw = malloc(sizeof(t_words));
	cuw->txt = ft_strdup(line);
	add_word_tofront(words, &cuw);
	return SUCCESS;
}

int filter_string(t_words **words, char *line, t_envs **exenv)
{
	t_strlen info_size_line;
	int i;
	int ret;
	t_words *keys = NULL;
	t_words *cuw;
	t_envs *var;
	int backtotal;
	int varsize;

	varsize = 0;
	backtotal = 0;
	i = -1;
	if (line[0] != 39)
	{
		while (line[++i])
		{
			if (line[i] == 92 && is_special(line[i + 1]))
			{
				backtotal++;
				i++;
			}else if (line[i] == '$'){
				i++;
				cuw = malloc(sizeof(t_words));
				get_var_name(line + i, &cuw->txt);
				addtmptowords(&keys, &cuw);
				var = get_env(&ret, cuw->txt, exenv);
				if (ret)
					varsize += ft_strlen(var->env_value);
				ret = ft_strlen(cuw->txt);
				backtotal += ret + 1;
				i += ret - 2;
			}
			i++;
		}
		info_size_line.len = ft_strlen(line) - backtotal + varsize;
		info_size_line.line = ft_strdup(line);
	}else{
		mk_and_add_to_words(words, line);
	}
	return SUCCESS;
}

int collect_strs(t_words **words, t_words *keys, t_envs **exenvs, t_strlen info)
{
	t_envs *cuvar;
	int i;
	int j;
	char *tmp;

	tmp = malloc(info.len + 1);
	i = -1;
	j = -1;
	while (line[++i])
	{	if (line[i] == '$')
		{
			i += ft_strlen(keys->txt);
			cuvar = get_env(&info.len, keys->txt, exenvs);
			if (info.len)
				j = help_fill_tmp(tmp, j, cuvar);
		}
		else if (line[i] == 92 && is_special(line[i + 1]))
			tmp[++j] = line[++i];
		else
			tmp[++j] = line[++i];
	}
	return SUCCESS;
}

int help_fill_tmp(char *tmp, char *value, int start)
{
	int i;

	i = -1;
	while (value[++i]){
		tmp[++start] = value[i];
	}
	return start; 
}

// "a \\ $varname|"
//
//
//
//
