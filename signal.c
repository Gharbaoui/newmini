#include "minishell.h"


int get_status(int st)
{
	int ret;

	if (WIFSIGNALED(st))
		ret = WTERMSIG(st) + 128;
	else
	ret = WEXITSTATUS(st); 
	//printf("%d\n", ret);
	return ret;
}


void handl_sig(int signum)
{
	if (signum == SIGINT)
	{
		if (glob_vars.childruning == 0)
		{
			if (glob_vars.line)
			{
				free(glob_vars.line);
			}
			write(1, "\nminishell=>", 12);
		}
		else{
			write(1, "\n", 1);	
		}
	}
	if (signum == SIGQUIT)
		printf("Cntrol + back\n");
}
