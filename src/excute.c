#include "minishell.h"

int	close_in_parent(int **pipe, int pindex)
{
	if (pindex > 0)
	{
		if (pindex > 1)
		{
			close(pipe[pindex - 2][READ_END]);
			close(pipe[pindex - 1][WRITE_END]);
		}
		else
			close(pipe[pindex - 1][WRITE_END]);
	}
	return (0);
}

int	prm_check(t_onecmd cmd)
{
	int		error;
	int		append;
	char	**fs;
	int		ret;

	ret = 0;
	if (cmd.files)
	{	
		fs = creat_w_files(cmd.files, cmd.ops, &error, &append);
		if (error)
		{
			ret = file_error(error, fs);
			fs = malloc(sizeof(char *));
		}
		free(fs);
	}
	if (cmd.cmd && cmd.prem)
		help_prem_check(cmd.cmd, &ret);
	return (ret);
}

void	help_prem_check(char *cmd, int *ret)
{
	dup2(2, 1);
	ft_printf(3, "bash: ", cmd, ": Permission denied\n");
	dup2(g_vars.fdout, 1);
	*ret = 126;
}

int	decide_in_out(int **pipe, char **files, char **ops, t_iter nums)
{
	char	**fs;
	int		append;
	int		error;

	if (files)
	{
		fs = creat_w_files(files, ops, &error, &append);
		if (error == 0)
			red_in_decide_files(fs, pipe, append, nums);
	}
	else
		red_in_decide_no_files(pipe, nums);
	if (nums.index < nums.count)
	{
		close(pipe[nums.index][WRITE_END]);
		close(pipe[nums.index][READ_END]);
	}
	return (0);
}

char	**creat_w_files(char **files, char **ops, int *error, int *append)
{
	int		i;
	int		fd;
	char	**fs;

	init_in_creat_wf(&fs, &i, error);
	while (files && files[++i])
	{
		if (short_creat_w(error, &fs[0], files[i]))
			break ;
		if (cre_write_files(&fs, files[i], ops[i], append) == 0)
		{
			if (*error == 0)
			{
				fd = open(files[i], O_RDONLY);
				if (fd < 0)
					*error = 1;
				else
					close(fd);
				fs[0] = files[i];
				if (*error)
					break ;
			}
		}
	}
	return (fs);
}
