#include "minishell.h"


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

int prm_check(t_onecmd cmd)
{
	int error;
	int append;
	char **fs;
	int ret;

	ret = 0;
	if (cmd.files)
	{	
			fs = creat_w_files(cmd.files, cmd.ops, &error, &append);	
			if (error)
			{
				printf("bash: %s: No such file or directory\n", fs[0]);
				ret = 1;
			}
			else if (cmd.cmd && cmd.prem)
			{
				printf("bash: %s: Permission denied\n", cmd.cmd);
				ret = 126;
			}
			free(fs);
	}
	return ret;	
}

int decide_in_out(int **pipe, char **files, char **ops, t_iter nums)
{
	char **fs;
    int append;
	int error;

	if (files)
	{
		fs = creat_w_files(files, ops, &error, &append);
		if (error == 0)
			red_in_decide_files(fs, pipe, append, nums);
		else
		{
			free(fs);
            printf("bash: %s: No such file or directory\n", fs[0]);
			return 1;
		}
	}
	else
		red_in_decide_no_files(pipe, nums);
    if (nums.index < nums.count)
    {
        close(pipe[nums.index][WRITE_END]);
        close(pipe[nums.index][READ_END]);
    }
	return 0;
}



char  **creat_w_files(char **files, char **ops, int *error, int *append) // returns last file
{
	int i;
	int fd;
	char **fs;
	
	init_in_creat_wf(&fs, &i, error);
	while (files && files[++i])
	{
		if (cre_write_files(&fs, files[i], ops[i], append) == 0)
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
