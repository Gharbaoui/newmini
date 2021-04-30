#include "minishell.h"
// ls ; cat -e >g; cat < g > l; ls | cat -e; ls > k
int fullexcute(t_completecmd **complete, t_fullvar **variables)
{
	t_pipcommand *pcmd;
	int check;
	t_completecmd *comp;

	comp = *complete;
	check = 0;
	while (comp)
	{
		if (glob_vars.envchanged || check == 0){
			glob_vars.envp  = update_env_var((*variables)->exenvs);
			glob_vars.envchanged = 0;
			check = 1;
		}
		pcmd = expand_current_command(comp, *variables);   ///// free here
		excute_one_cmd(pcmd, variables);
		if (glob_vars.envchanged)
			free_dstr(glob_vars.envp);
		comp = comp->next;
	}
	return SUCCESS;
}

int excute_one_cmd (t_pipcommand *pcmd, t_fullvar **variables)
{
	int **pipes;
	t_iter nums;
	
    nums.count = get_num_subcmds(pcmd) - 1;
    if (nums.count > 0)
	{
        alloc_pipes(&pipes, nums.count);
		nums.index = 0;
        ex_mu_p_cmd(pcmd, pipes, variables, nums);
    }else{
		run_sim_cmd(pcmd->cmd, variables);
	}
	update_exit_status((*variables)->exenvs);
	return SUCCESS;
}

void update_exit_status(t_envs **exenvs)
{
	t_envs *cvar;
	int found;
	
	cvar = get_env(&found, "?", exenvs);
	free(cvar->env_value);
	cvar->env_value = ft_itoa(glob_vars.exitstatus);
}


int run_exact_cmd(t_onecmd cmd, t_fullvar **env_var)
{
	if (!builtin(cmd.cmd)){
    	execve(cmd.cmd, cmd.args, glob_vars.envp);
    	return -1;
	}
	return run_built_in(cmd, env_var);
}




void overwrite_or_append(int append, char **fs)
{
	int fd;

	if (append == 0)
		fd = open (fs[1], O_WRONLY);
	else
		fd = open(fs[1], O_WRONLY | O_APPEND);
	dup2(fd, 1);
	close(fd);
}


void red_in_decide_files(char **fs, int **pipe, int append, t_iter nums)
{
	int fd;

	if (fs[1])
	{
		overwrite_or_append(append, fs);
	}else
	{
		if (nums.index < nums.count)
			dup2(pipe[nums.index][WRITE_END], 1);
	}
	if (fs[0])
	{
		fd = open (fs[0], O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}else if (nums.index > 0)
	{
		dup2(pipe[nums.index - 1][READ_END], 0);
		close(pipe[nums.index - 1][READ_END]);
	}
}


void red_in_decide_no_files(int **pipe, t_iter nums)
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


int cre_write_files(char ***fs, char *file, char *op, int *append)
{
	int ret;
	int fd;

	ret = 0;
	if (ft_cmpstr(op, ">")){
		ret = 1;
		fd = open (file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		(*fs)[1] = file;
		*append = 0;
		close(fd);
	}
	else if (ft_cmpstr(op, ">>"))
	{
		ret = 1;
		fd = open (file, O_WRONLY | O_APPEND | O_CREAT, 0644);
		(*fs)[1] = file;
        *append = 1;
		close(fd);
	}
	return ret;
}

void init_in_creat_wf(char ***fs, int *i, int *error)
{
	*fs = malloc(sizeof(char *) * 2);
	*i = -1;	
	*error = 0;
	(*fs)[0] = NULL;
	(*fs)[1] = NULL;
}
