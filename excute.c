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
			printf("bash: %s: No such file or directory\n", fs[0]);
			ret = 1;
		}
		free(fs);
	}
	else if (cmd.cmd && cmd.prem)
	{
		printf("bash: %s: Permission denied\n", cmd.cmd);
		ret = 126;
	}
	return (ret);
}
