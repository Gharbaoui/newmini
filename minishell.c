#include "minishell.h"
int main(int argc, char **argv, char **env)
{
	printf("%d\n", getpid());
	bash_loop(env);
}

void bash_loop(char **env)
{
	int status;
	int ret;
	char *line = malloc(1);
	t_completecmd *complete;
	
	status = 1;
	while (status)
	{
		free(line);
		write(1, "minishell=>", 11);
		ret = get_next_line(&line);
		if (ret > 0)
		{
			ret =  workon_line(line, &complete, 0, 0);
			if (ret == PARSERROR)
				printf("Parsing Error\n");
		}

	}
}
