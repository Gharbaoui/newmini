#include "minishell.h"

int run_sim_ifcmd(t_onecmd cmd, t_fullvar **env_var)
{
	int status;
	int pid;
	
    if (!builtin(cmd.cmd))
   	{
        pid = fork();
       	if (pid == 0)
       	{
           	status = actual_exec_one(cmd, env_var);
         	exit(status);
        }
       	waitpid(pid, &status, 0);
		glob_vars.exitstatus = WEXITSTATUS(status);
       	return status;
    }
	else
		run_built_in(cmd, env_var);
	return -1999;
}

void help_handl_red(char **fs, int append)
{
	int fd[2];
	
	if (fs[1])
	{
		if (append == 0)
			fd[1] = open (fs[1], O_WRONLY);
		else
			fd[1] = open(fs[1], O_WRONLY | O_APPEND);
		dup2(fd[1], 1);
		close(fd[1]);
	}
	if (fs[0])
	{
		fd[0] = open (fs[0], O_RDONLY);
		dup2(fd[0], 0);
		close(fd[0]);
	}	
}
