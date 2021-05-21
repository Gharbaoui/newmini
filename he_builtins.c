#include "minishell.h"

int h1_cd(char **home, char **paths, t_fullvar **vars)
{
	t_envs *var;
	int i;
	*home = NULL;
	glob_vars.exitstatus = 0;
	if (paths[1] && paths[2])
	{
		glob_vars.exitstatus = 1;
		printf("bash: cd: too many arguments\n");
		return 1;
	}
	var = get_env(&i, "HOME", (*vars)->exenvs);
	if (i)
		*home = var->env_value;
	return 0;	
}

int h2_cd(char *home)
{
	int check;

	check = chdir(home);
	if (check)
	{
		glob_vars.exitstatus = 1;
		perror("bash: cd");
		return 1;
	}
	return 0;
}
