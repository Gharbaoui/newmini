#include "minishell.h"



int run_child(t_onecmd cmd, int **pipe, t_iter nums, t_fullvar **env_var)
{
	int status;
	status = exc_one_cmd(cmd, pipe, nums, env_var);
	exit(status);
	
}

void default_fds(int *stdo, int *stdi)
{
	*stdi = dup(0);
	*stdo = dup(1);
}


int exc_one_cmd(t_onecmd cmd, int **pipe, t_iter nums, t_fullvar **env_var)
{
    int ret;
    int orgin[2];

    default_fds(&orgin[1], &orgin[0]);
    ret = decide_in_out(pipe, cmd.files, cmd.ops, nums);
    if (ret != 0) // means that file is mising
        return 1;
    if (cmd.cmd)
    {
        run_exact_cmd(cmd, env_var);
        dup2(orgin[1], 1);
        dup2(orgin[0], 0);
        if (cmd.cmd[0] == '/' || cmd.cmd[0] == '~' || cmd.cmd[0] == '.')
            printf("bash: %s: No such file or directory\n", cmd.cmd);
        else
            printf("%s: command not found\n", cmd.cmd);
        return 127; // for command not found
    }
    return 0;
}


int ex_mu_p_cmd(t_pipcommand *pcmd, int **pipe, t_fullvar **env_var, t_iter nums)
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
			run_child(cmd, pipe, nums, env_var);
        if (pcmd->next == NULL)
        {
            waitpid(pid, &status, 0);
            close(pipe[nums.index - 1][READ_END]);
            return status;
        }else{
            nums.index++;
            status = ex_mu_p_cmd(pcmd->next, pipe, env_var, nums);
            wait(NULL);
            if (status != -1999)
                return status;
            return -1999;
        }
	}
	return -1999;
}