/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 21:29:08 by aez-zaou          #+#    #+#             */
/*   Updated: 2021/06/18 21:29:09 by aez-zaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fullexcute(t_completecmd **complete, t_fullvar **variables)
{
	t_pipcommand	*pcmd;
	t_completecmd	*comp;

	comp = *complete;
	while (comp)
	{
		if (g_vars.envchanged)
		{
			g_vars.envp = update_env_var((*variables)->exenvs);
			g_vars.envchanged = 0;
		}
		pcmd = expand_current_command(comp, *variables);
		excute_one_cmd(pcmd, variables);
		if (g_vars.envchanged)
		{
			free_dstr(g_vars.envp);
			free(g_vars.envp);
		}
		free_laststr(&pcmd);
		if (g_vars.exit)
			break ;
		comp = comp->next;
	}
	free_comp(complete);
	return (SUCCESS);
}

int	excute_one_cmd (t_pipcommand *pcmd, t_fullvar **variables)
{
	int		**pipes;
	t_iter	nums;

	g_vars.exit = 0;
	nums.count = get_num_subcmds(pcmd) - 1;
	if (nums.count > 0)
	{
		alloc_pipes(&pipes, nums.count);
		nums.index = 0;
		ex_mu_p_cmd(pcmd, pipes, variables, nums);
		free_pipes(pipes, nums.count);
	}
	else
		run_sim_cmd(pcmd->cmd, variables);
	update_exit_status((*variables)->exenvs);
	return (SUCCESS);
}

void	update_exit_status(t_envs **exenvs)
{
	t_envs	*cvar;
	int		found;

	cvar = get_env(&found, "?", exenvs);
	free(cvar->env_value);
	cvar->env_value = ft_itoa(g_vars.exitstatus);
}

int	run_exact_cmd(t_onecmd cmd, t_fullvar **env_var)
{
	if (!builtin(cmd.cmd, cmd.args[0]))
	{
		execve(cmd.cmd, cmd.args, g_vars.envp);
		return (-1);
	}
	return (run_built_in(cmd, env_var));
}

void	overwrite_or_append(int append, char **fs)
{
	int	fd;

	if (append == 0)
		fd = open (fs[1], O_WRONLY);
	else
		fd = open(fs[1], O_WRONLY | O_APPEND);
	dup2(fd, 1);
	close(fd);
}
