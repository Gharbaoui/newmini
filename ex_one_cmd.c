#include "minishell.h"

int run_sim_cmd(t_onecmd cmd, t_fullvar **env_var)
{
    int pid;
    int status;

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
    }else
		run_built_in(cmd, env_var);
    return 0;
}

int run_built_in(t_onecmd cmd, t_fullvar **vars)
{
	char *lcmd;

	lcmd = lower_str(cmd.cmd);
	if (ft_strcmp(lcmd, "export") == 0)
		return ft_export(cmd.args, vars);
}



int handl_red(t_onecmd cmd)
{
    char **fs;
    int fd[2];
    int error;
    int append;
    
    if (cmd.files)
    {
        fs = creat_w_files(cmd.files, cmd.ops, &error, &append);
        if (error == 0)
        {
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
        else{
            printf("bash: %s: No such file or directory\n", fs[0]);
            return 1; // error happend
        }
    }
    return 0;   
}

int actual_exec_one(t_onecmd cmd, t_fullvar **env_var)
{
    int origin[2];

    origin[1] = dup(1);
    origin[0] = dup(0);
    if (handl_red(cmd))
        return 1;
    if (cmd.cmd)
    {
		if (cmd.prem != 0){
			printf("bash: %s: Permission denied\n", cmd.cmd);
			return 126;
		}
        run_exact_cmd(cmd, env_var);
        dup2(origin[1], 1);
        dup2(origin[0], 0);
        if (cmd.cmd[0] == '/' || cmd.cmd[0] == '~' || cmd.cmd[0] == '.')
            printf("bash: %s: No such file or directory\n", cmd.cmd);
        else
            printf("%s: command not found\n", cmd.cmd);
        return 127; // for command not found
    }
    return 0;
}

int builtin(char *cmd)
{
	char *lcmd;
	int ret;
	
	ret = 0;
	lcmd = lower_str(cmd);
	if (ft_cmpstr(lcmd, "echo") || ft_cmpstr(lcmd, "export") || ft_cmpstr(lcmd, "cd"))
    	ret = 2;
	else if (ft_cmpstr(lcmd, "exit") || ft_cmpstr(lcmd, "unset") || ft_cmpstr(lcmd, "env") || ft_cmpstr(lcmd, "pwd"))
		ret = 2;
	free(lcmd);
    return ret;
}


