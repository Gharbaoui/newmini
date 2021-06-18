#include "minishell.h"

void	red_in_decide_files(char **fs, int **pipe, int append, t_iter nums)
{
	int	fd;

	if (fs[1])
		overwrite_or_append(append, fs);
	else
		if (nums.index < nums.count)
			dup2(pipe[nums.index][WRITE_END], 1);
	if (fs[0])
	{
		fd = open (fs[0], O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	else if (nums.index > 0)
	{
		dup2(pipe[nums.index - 1][READ_END], 0);
		close(pipe[nums.index - 1][READ_END]);
	}
	free(fs);
}

void	red_in_decide_no_files(int **pipe, t_iter nums)
{
	if (nums.index < nums.count)
		dup2(pipe[nums.index][WRITE_END], 1);
	if (nums.index > 0)
	{
		dup2(pipe[nums.index - 1][READ_END], 0);
		close(pipe[nums.index - 1][READ_END]);
	}
}

int	cre_write_files(char ***fs, char *fileor, char *op, int *append) ///// changed
{
	int		ret;
	int		fd;
	char	*file;

	file = fileor;
	file++;
	file[ft_strlen(file) - 1] = 0;
	ret = 0;
	if (ft_cmpstr(op, ">"))
	{
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
	return (ret);
}

void	init_in_creat_wf(char ***fs, int *i, int *error)
{
	*fs = malloc(sizeof(char *) * 2);
	*i = -1;
	*error = 0;
	(*fs)[0] = NULL;
	(*fs)[1] = NULL;
}
