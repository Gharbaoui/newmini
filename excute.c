#include "minishell.h"

int fullexcute(t_completecmd **complete, t_fullvar **variables, t_prstatus *prstatus)
{
    t_pipcommand *pcmd;
    t_completecmd *cmpt;

    cmpt = *complete;
    while (cmpt)
    {
        pcmd = expand_current_command(cmpt, *variables);
        excute_one_cmd(pcmd, variables);
        cmpt = cmpt->next;
    }
}

int excute_one_cmd(t_pipcommand *pcmd, t_fullvar **variables)
{
    int count;
    int **pipes;

    count = get_num_subcmds(pcmd);
    if (count > 1){
        alloc_pipes(&pipes, count);
        exec_multi_pipe(pcmd, pipes, variables, 0);
    }
    
    
}

int exec_multi_pipe(t_pipcommand *pcmd, int **pipe, t_fullvar **variables, int index)
{
  	int pid;
	int fd[2]; // file descripto for write and read files  1 for > or >>  0 for < 
	int error;
	t_onecmd cmd;
	char **fs;

	pid = fork();
	cmd = pcmd->cmd;
	if (pid == 0)
	{
		if (cmd.files)
		{
			fs = creat_w_files(cmd.files, cmd.ops, &error);
			if (error == 0)
			{
				fd[1] = open (fs[1], O_WRONLY);
				dup2(fd[1], 1);
				fd[0] = open (fs[0], O_RDONLY);
				dup2(fd[0], 0);
			}
		}
		
	}	
}

char  **creat_w_files(char **files, char **ops, int *error) // returns last file
{
	int i;
	int fd;
	char **fs;

	fs = malloc(sizeof(char *) * 2);
	i = -1;
	*error = 0;
	while (files[++i])
	{
		if (ft_cmpstr(ops[i], ">")){
			fd = open (files[i], O_WRONLY | O_APPEND | O_CREAT, 0644);
			fs[1] = files[i];
			close(fd);
		}else if (ft_cmpstr(ops[i], ">>"))
		{
			fd = open (files[i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
			fs[1] = files[i];
			close(fd);
		}
		else
		{
			if (*error == 0)
			{
				fd = open(files[i], O_RDONLY);
				if (fd < 0)
					*error = 1;
				else
					close(fd);
				fs[0] = files[i];
			}
		}
	}
	return fs;
}

void alloc_pipes(int ***pipes, int count)
{
    int i;

    i = -1;
    *pipes = malloc(sizeof(int *) * (count - 1));
    while (++i < count - 1){
        (*pipes)[i] = malloc(sizeof(int) * 2);
		pipe((*pipes)[i]);
	}
}

int get_num_subcmds(t_pipcommand *pcmd)
{
    int i;

    i = 0;
    while (pcmd)
    {
        i++;
        pcmd = pcmd->next;
    }
    return i;
}
