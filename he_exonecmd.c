#include "minishell.h"


int run_sim_ifcmd(t_onecmd cmd, t_fullvar **env_var)
{
	int status;
	int pid;
	int def[2];

	default_fds(&def[1], &def[0]);
    if (!builtin(cmd.cmd, cmd.args[0]))
   	{
		glob_vars.childruning = 1;
        pid = fork();
       	if (pid == 0)
       	{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
           	status = actual_exec_one(cmd, env_var);
         	exit(status);
        }
       	waitpid(pid, &status, 0);
		glob_vars.childruning = 0;
		glob_vars.exitstatus = get_status(status);
       	return status;
    }
	else
	{ 
		exc_one_built(cmd, env_var);
        dup2(def[1], 1);
        dup2(def[0], 0);
	}
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
	free(fs);
}
