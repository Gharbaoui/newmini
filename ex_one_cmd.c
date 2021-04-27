#include "minishell.h"

int run_sim_cmd(t_onecmd cmd, t_fullvar **env_var)
{
	char **fs;
    int status;
	int error;
	
	if (cmd.cmd)
	{
		status = run_sim_ifcmd(cmd, env_var);
		if (status != -1999)
			return status;
	}
	else
	{
		fs = creat_w_files(cmd.files, cmd.ops, &error, &status);
		if (error)
		{
        	printf("bash: %s: No such file or directory\n", fs[0]);
			glob_vars.exitstatus = 1;
        	return 1; // error happend
		}
	}
    return 0;
}

int run_built_in(t_onecmd cmd, t_fullvar **vars)
{
	char *lcmd;
	
	lcmd = lower_str(cmd.cmd);
	if (ft_strcmp(lcmd, "export") == 0)
	{
		free(lcmd);
		return ft_export(cmd.args, vars);
	}
	if (ft_strcmp(lcmd, "echo") == 0)
	{
		free(lcmd);
		return _echo(cmd.args);
	}
}



int handl_red(t_onecmd cmd)
{
    char **fs;
    int error;
    int append;
    
    if (cmd.files)
    {
        fs = creat_w_files(cmd.files, cmd.ops, &error, &append);
        if (error == 0)
			help_handl_red(fs, append);
        else
		{
            printf("bash: %s: No such file or directory\n", fs[0]);
            return 1; // error happend
        }
    }
    return 0;   
}

int actual_exec_one(t_onecmd cmd, t_fullvar **env_var)
{
    int origin[2];
	
	origin[0] = prm_check(cmd);
	if (origin[0] != 0)
		return origin[0];
    origin[1] = dup(1);
    origin[0] = dup(0);
    if (handl_red(cmd))
        return 1;
    if (cmd.cmd)
    {
        execve(cmd.cmd, cmd.args, glob_vars.envp);
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


