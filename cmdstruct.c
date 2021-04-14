#include "minishell.h"

t_multcmd *get_cmd_struct(t_completecmd *cmpcmd)
{
	t_multcmd mucmd;
	t_multcmd *head;
	
	head = &mucmd;
	while (cmpcmd)
	{
		fill_onepipcmd(&mucmd.pipcmd, cmpcmd->splcommand);
		mucmd.next = malloc(sizeof(t_multcmd));
		mucmd = *mucmd.next;
		cmpcmd = cmpcmd->next;
		mucmd.next = NULL;
	}
	return head;
}

int fill_onepipcmd(t_pipcommand *pipcmd, t_pipcmd *pip)
{
	while (pip)
	{
		fill_one_cmd(&pipcmd->cmd, pip->cmd);
		pipcmd->next = malloc(sizeof(t_pipcommand));
		pipcmd = pipcmd->next;
		pip = pip->next;
	}
	return SUCCESS;
}


int fill_one_cmd(t_onecmd *fcmd, t_cmd *pcmd)
{
	if (pcmd)
	{
		fcmd->cmd = pcmd->command;
		fcmd->args = transfrm_ln_arr(pcmd->txts, pcmd->command, 1);
		fcmd->files = transfrm_ln_arr(pcmd->files, pcmd->command, 0);
		fcmd->ops = transfrm_ln_arr(pcmd->ops, pcmd->command, 0);
	}else
		fcmd = NULL;;
	return SUCCESS;
}


char **transfrm_ln_arr(t_words *words, char *cmd, int iscmd)
{
	char **allw;
	int len;
	int i;
	
	len = how_many_words(words);
	allw = malloc(sizeof(char *) * (len + 1 + iscmd));
	i = -1;
	if (iscmd)
		allw[++i] = ft_strdup(cmd);
	while (++i < len)
	{
		allw[i] = ft_strdup(words->txt);
		words = words->next;
	}
	allw[i] = NULL;
	return allw;
}

int how_many_words(t_words *words)
{
	int i;

	i = 0;
	while (words)
	{
		words = words->next;
		i++;
	}
	return i;
}


void print_cmds(t_multcmd *cmd)
{
	while (cmd)
	{
		print_p(cmd->pipcmd);
		cmd = cmd->next;
	}
}

void print_p(t_pipcommand pip)
{
	while (pip)
	{
		print_c(pip.cmd);
		pip = *pip.next;
	}
}

void print_c(t_onecmd cmd)
{
		
}

void print_arr(char **str)
{
	int i;

	i = -1;
	while (str[i])
	{
		printf("%s, ", );
	}
}
