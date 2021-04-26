#include "minishell.h"

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
		pcmd = expand_current_command(comp, *variables);
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
