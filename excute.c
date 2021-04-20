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
    int **pipes;
	t_iter nums;
    int ret;

    nums.count = get_num_subcmds(pcmd) - 1;
    if (nums.count > 0){
        alloc_pipes(&pipes, nums.count);
		nums.index = 0;
        ret = exec_multi_pipe(pcmd, pipes, variables, nums);
        printf("%d\n",WEXITSTATUS(ret));
    }
    
    
}

int exec_multi_pipe(t_pipcommand *pcmd, int **pipe, t_fullvar **variables, t_iter nums)
{
    t_onecmd cmd;
    int pid;
    int status;
    if (pcmd)
    {
        cmd = pcmd->cmd;
        close_in_parent(pipe, nums.index);
        pid = fork();
        if (pid == 0)
        {
            if (!decide_in_out(pipe, cmd.files, cmd.ops, nums))
                run_command(cmd);
            //exit(20);  //// testing
        }
        if (pcmd->next == NULL)
            close(pipe[nums.index - 1][READ_END]);
        nums.index++;
        exec_multi_pipe(pcmd->next, pipe, variables, nums);
        if (pcmd->next == NULL)
        {
            waitpid(pid, &status, 0);
            return status;
        }else
            wait(NULL);
    }
}

int run_command(t_onecmd cmd)
{
	if (cmd.cmd)
	{
        execve(cmd.cmd, cmd.args, NULL);
	}
	
}


int close_in_parent(int **pipe, int pindex)
{
    if (pindex > 0)
    {
        if (pindex > 1)
        {
            close(pipe[pindex - 2][READ_END]);
            close(pipe[pindex - 1][WRITE_END]);
        }else
            close(pipe[pindex - 1][WRITE_END]);
    }
    return 0;
}

int decide_in_out(int **pipe, char **files, char **ops, t_iter nums)
{
	char **fs;
    int append;
	int error;
	int fd[2];

	if (files)
	{
		fs = creat_w_files(files, ops, &error, &append);
		if (error == 0)
		{
			if (fs[1]){
                if (append == 0)
				    fd[1] = open (fs[1], O_WRONLY);
                else
                    fd[1] = open(fs[1], O_WRONLY | O_TRUNC);
				dup2(fd[1], 1);
				close(fd[1]);
			}else
			{
				if (nums.index < nums.count)
				{
					dup2(pipe[nums.index][WRITE_END], 1);
				}
			}
			if (fs[0])
			{
				fd[0] = open (fs[0], O_RDONLY);
				dup2(fd[0], 0);
				close(fd[0]);
			}else{
				if (nums.index > 0)
				{
					dup2(pipe[nums.index - 1][READ_END], 0);
					close(pipe[nums.index - 1][READ_END]);
				}
			}
		}else{ /// here error happend
            printf("bash: %s: No such file or directory\n", fs[0]);
			return 1;
		}
	}else
	{
		if (nums.index < nums.count)
		{
			dup2(pipe[nums.index][WRITE_END], 1);
		}
		if (nums.index > 0)
		{
			dup2(pipe[nums.index - 1][READ_END], 0);
            close(pipe[nums.index - 1][READ_END]);
        }
	}
    if (nums.index < nums.count)
    {
        close(pipe[nums.index][WRITE_END]);
        close(pipe[nums.index][READ_END]);
    }
	return 0;
}

void close_write_rest(int **pipes, int index, int pipsize)
{
	if (index < pipsize)
	{
		close(pipes[index][WRITE_END]);
	}
}

void close_read_rest(int **pipes, int index, int pipsize)
{
	if (index > 0)
	{
		close(pipes[index - 1][READ_END]);
	}
}

void close_pipes(int **pipes, int index, int pipslen)
{
	int i;

	i = -1;
	if (index == 0)
	{
		i = index;
		close(pipes[0][READ_END]);
		while (++i < pipslen)
		{
			close(pipes[i][READ_END]);
			close(pipes[i][WRITE_END]);
		}
	}else if (index < pipslen)
	{
		while (++i < pipslen)
		{
			if (i + 1 != index && i != index)
			{
				close(pipes[i][READ_END]);
				close(pipes[i][WRITE_END]);
			}else if (i + 1 == index)
				close(pipes[i][WRITE_END]);
			else
				close(pipes[i][READ_END]);
		}
	}else{
		while (++i < pipslen)	
		{
			if (i + 1 != index)
			{
				close(pipes[i][READ_END]);
				close(pipes[i][WRITE_END]);
			}else
				close(pipes[i][WRITE_END]);
		}
	}
}

char  **creat_w_files(char **files, char **ops, int *error, int *append) // returns last file
{
	int i;
	int fd;
	char **fs;

	fs = malloc(sizeof(char *) * 2);
	i = -1;
	*error = 0;
	fs[0] = NULL;
	fs[1] = NULL;
    *append = 0;
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
            *append = 1;
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
    *pipes = malloc(sizeof(int *) * (count));
    while (++i < count){
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
