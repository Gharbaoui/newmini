#include "minishell.h"
int main(int argc, char **argv, char **env)
{
	bash_loop(env);
}

void bash_loop(char **env)
{
	signal(SIGINT, handl_sig);
	signal(SIGQUIT, handl_sig);
	t_fullvar *variables;
    t_prstatus prstatus;
	int status;
	int ret;
	t_envs **envtable;
	t_completecmd *complete;
	glob_vars.envchanged = 1;
	variables = malloc(sizeof(t_fullvar));
	variables->exenvs = NULL;
	variables->filledvar = NULL;
	variables->allkeys = NULL;
	ret = fill_envtable(&variables, env);// returns Success or memory error or Empty env
	
	add_toenvtable(&variables, "?=0");
	status = 1;
	glob_vars.line = malloc(1);
	glob_vars.childruning = 0;
	while (status)
	{
		free(glob_vars.line);
		glob_vars.line = NULL;
		write(1, "minishell=>", 11);
		ret = get_next_line(&glob_vars.line);
		// here i need to handel inputs to my bash
		if (ret > 0)
		{
			ret =  workon_line(glob_vars.line, &complete, 0, 0);
			if (ret == PARSERROR)
            {
				printf("Parsing Error\n");
                free_comp(&complete);
            }else{
                fullexcute(&complete, &variables);
            }
		}else if (ret == 0)
		{
			printf("exited\n");
			status = 0;
		}

	}
}


