#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	argc = 0;
	argv = NULL;
	bash_loop(env);
}

void bash_loop(char **env)
{
	signal(SIGINT, handl_sig);
	signal(SIGQUIT, handl_sig);
	t_fullvar *variables;
	int status;
	int ret;
	t_completecmd *complete;
	variables = malloc(sizeof(t_fullvar));
	variables->exenvs = NULL;
	variables->filledvar = NULL;
	variables->allkeys = NULL;
	ret = fill_envtable(&variables, env);// returns Success or memory error or Empty env
	
	add_toenvtable(&variables, "?=0");
	status = 1;
	glob_vars.envchanged = 1;
	glob_vars.line = malloc(1);
	glob_vars.childruning = 0;
	glob_vars.fdout = dup(1);
	glob_vars.exenvs = &variables->exenvs;
	glob_vars._pwd = NULL;
	update_pwd(variables->exenvs);
	level_of_bash(variables->exenvs);
	while (status)
	{
		free(glob_vars.line);
		glob_vars.line = NULL;
		write(1, "minishell > ", 12);
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
				if (glob_vars.exit)
				{
					free(glob_vars.line);
					exit(glob_vars.exitstatus);
				}
            }
		}else if (ret == 0)
		{
			printf("exited\n");
			status = 0;
		}
	}
	close(glob_vars.fdout);
}


