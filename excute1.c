#include "minishell.h"

int fullexcute(t_completecmd **complete, t_fullvar **variables)
{
	t_pipcommand *pcmd;
	t_completecmd *comp;

	comp = *complete;
	while (comp)
	{
		glob_vars.envp  = update_env_var((*variables)->exenvs);
		pcmd = expand_current_command(comp, *variables);
		excute_one_cmd(pcmd, variables);
		// free envp variable here
		comp = comp->next;
	}	
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
        glob_vars.exitstatus = ex_mu_p_cmd(pcmd, pipes, variables, nums);
		glob_vars.exitstatus = WEXITSTATUS(glob_vars.exitstatus);
    }else{
		run_sim_cmd(pcmd->cmd, variables);
	}
	printf("%d\n", 	glob_vars.exitstatus);
}




int run_exact_cmd(t_onecmd cmd, t_fullvar **env_var)
{
    execve(cmd.cmd, cmd.args, glob_vars.envp);
    return -1;
}
