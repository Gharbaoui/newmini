#include "minishell.h"

int	run_sim_cmd(t_onecmd cmd, t_fullvar **env_var)
{
	char	**fs;
	int		status;
	int		error;

	if (cmd.cmd)
	{
		status = run_sim_ifcmd(cmd, env_var);
		if (status != -1999)
			return (status);
	}
	else if (cmd.ops)
	{
		fs = creat_w_files(cmd.files, cmd.ops, &error, &status);
		if (error)
			return (file_error(error, fs));
		free(fs);
	}
	return (0);
}

int	file_error(int err, char **fs)
{//// new function
	dup2(2, 1);
	if (err == 3)
		ft_printf(3, "bash: ", fs[0], ": ambiguous redirect\n");
	else
		ft_printf(3, "bash: ", fs[0], ": No such file or directory\n");
	g_vars.exitstatus = 1;
	dup2(1, g_vars.fdout);
	free(fs);
	return (1);
}

int	handl_red(t_onecmd cmd)
{
	char	**fs;
	int		error;
	int		append;

	if (cmd.files)
	{
		fs = creat_w_files(cmd.files, cmd.ops, &error, &append);
		if (error == 0)
			help_handl_red(fs, append);
		else
			return (file_error(error, fs));
	}
	return (0);
}

int	actual_exec_one(t_onecmd cmd)
{
	int	ret;

	ret = prm_check(cmd);
	if (ret != 0)
		return (ret);
	if (handl_red(cmd))
		return (1);
	if (cmd.cmd)
	{
		execve(cmd.cmd, cmd.args, g_vars.envp);
		dup2(2, 1);
		if (cmd.cmd[0] == '/' || cmd.cmd[0] == '~' || cmd.cmd[0] == '.')
			ft_printf(3, "bash: ", cmd.cmd, ": No such file or directory\n");
		else
			ft_printf(2, cmd.args[0], ": command not found\n");
		ret = 127;
		dup2(1, g_vars.fdout);
	}
	return (ret);
}

int	exc_one_built(t_onecmd cmd, t_fullvar **env_var)
{
	if (handl_red(cmd))
		return (1);
	if (cmd.cmd)
		return (run_built_in(cmd, env_var));
	return (0);
}

int	builtin(char *cmd, char *origin)
{
	char	*lcmd;
	int		ret;

	ret = 0;
	lcmd = lower_str(cmd);
	if (ft_cmpstr(lcmd, "echo") || ft_cmpstr(lcmd, "export")
		|| ft_cmpstr(lcmd, "cd"))
		ret = 2;
	else if (ft_cmpstr(origin, "exit") || ft_cmpstr(lcmd, "unset")
		|| ft_cmpstr(lcmd, "env") || ft_cmpstr(lcmd, "pwd"))
		ret = 2;
	free(lcmd);
	return (ret);
}
