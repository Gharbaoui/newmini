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
			glob_vars.exitstatus = 1;
			update_exit_status(*glob_vars.exenvs);
			if (glob_vars.line)
			{
				free(glob_vars.line);
			}
			write(1, "\nminishell=>", 13);
		}
		else{
			write(1, "\n", 1);	
		}
	}
	if (signum == SIGQUIT)
	{
		if (glob_vars.childruning)
		{
			write(1, "QUIT 3\n", 7);
		}
	}
}
