#include "minishell.h"
int main(int argc, char **argv, char **env)
{
	bash_loop(env);
}

void bash_loop(char **env)
{
	t_fullvar *variables;
    t_prstatus prstatus;
	int status;
	int ret;
	t_envs **envtable;
	char *line = malloc(1);
	t_completecmd *complete;

	variables = malloc(sizeof(t_fullvar));
	variables->exenvs = NULL;
	variables->filledvar = NULL;
	variables->allkeys = NULL;
	ret = fill_envtable(&variables, env);// returns Success or memory error or Empty env
	
	status = 1;
	add_toenvtable(&variables, "?=0");
	/*ft_export(&variables, "new=");
	print_words(variables->allkeys, 0, "");
	print_words(variables->filledvar, 0, "");*/

	while (status)
	{
		free(line);
		line = NULL;
		write(1, "minishell=>", 11);
		ret = get_next_line(&line);
		// here i need to handel inputs to my bash
		if (ret > 0)
		{
			ret =  workon_line(line, &complete, 0, 0);
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


