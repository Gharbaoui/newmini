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
	ret = prm_check(cmd);
	if (ret != 0)
		return ret;
    ret = decide_in_out(pipe, cmd.files, cmd.ops, nums);
    if (ret != 0) // means that file is mising
        return 1;
    if (cmd.cmd)
    {
        ret = run_exact_cmd(cmd, env_var);
		if (ret != -1)
			return ret;
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

int help_ex_mu(int pid, int **pipe, int numindex)
{
	int status;
	close(pipe[numindex - 1][READ_END]);
	waitpid(pid, &status, 0);
	glob_vars.exitstatus = WEXITSTATUS(status);
	return status;
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
		glob_vars.childruning = 1;
		pid = fork();
		if (pid == 0)
			run_child(cmd, pipe, nums, env_var);
        if (pcmd->next == NULL)
			return help_ex_mu(pid, pipe, nums.index);
		else
		{
            nums.index++;
            status = ex_mu_p_cmd(pcmd->next, pipe, env_var, nums);
            wait(NULL);
            if (status != -1999)
                return status;
            return -1999;
        }
	}
	glob_vars.childruning = 0;
	return -1999;
}


