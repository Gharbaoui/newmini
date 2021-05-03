#include "./minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>




//////////////////////////// CD ////////////////////////////
int cd(char **paths, t_fullvar **vars)
{
	int i;
	t_envs *var;
	char *home;

	home = NULL;
	glob_vars.exitstatus = 0;
	if (paths[1] && paths[2])
	{
		glob_vars.exitstatus = 1;
		printf("bash: cd: too many arguments\n");
		return 1;
	}
	var = get_env(&i, "HOME", (*vars)->exenvs);
	if (i)
		home = var->env_value;
	if (paths[1] == NULL && home == NULL) // TODO: to change getenv to the builtin export
	{
		printf("Minishell: cd: HOME not set\n");
		glob_vars.exitstatus = 1;
		return (1);
	}
	else if (paths[1])
		home = paths[1];
	else if (home[0] == '\0')
		home = ".";
	i = chdir(home);
	if (i)
	{
		glob_vars.exitstatus = 1;
		perror("minishell: cd");
	}
	set_pwd(vars);
	return (i);
}



void change_pwd_old_pwd(t_fullvar **vars, char *opwd)
{
	t_envs *var;
	char *help;
	char **oldpwd_newpwd;
	char *path;

	oldpwd_newpwd = malloc(sizeof(char *) * 4);
	help = ft_strdup("OLDPWD=");
	oldpwd_newpwd[1] = ft_strjoin(&help, opwd);
	path = malloc(1000);
	help = ft_strdup("PWD=");
	getcwd(path, 1000);
	oldpwd_newpwd[2] = ft_strjoin(&help, path);
	free(path);
	oldpwd_newpwd[3] = NULL;
	ft_export(oldpwd_newpwd, vars);
	free(oldpwd_newpwd[1]);
	free(oldpwd_newpwd[2]);
	free(oldpwd_newpwd);
}

void set_pwd(t_fullvar **vars)
{
	int found;
	t_envs *var;

	var = get_env(&found, "PWD", (*vars)->exenvs);
	if (found)
		change_pwd_old_pwd(vars, var->env_value);
	else
	{
	 	// remove oldpwd and run export(OLDPWD)
		unset_one_var("OLDPWD", vars);
		sub_export(vars, "OLDPWD");	
	}
}
//-------------------------------------------------------


/////   env
int ft_env (t_fullvar *vars)
{
	t_envs *var;
	t_words *help;
	int found;

	help = vars->filledvar;
	glob_vars.exitstatus = 0;
	while (help)
	{
		var = get_env(&found, help->txt, vars->exenvs);
		printf("%s=%s\n", var->env_name, var->env_value);
		help = help->next;
	}
	return 0;
}
//////

////// exit
int ft_exit(char **args)
{
	glob_vars.exit = 1;
	return 0;
}
///////
//////////////////////////// echo ////////////////////////////
int     _echo(char **args)
{
    int new_line;
	int i;

	glob_vars.exitstatus = 0;
    new_line = 0;
	i = 1;
    while (args[i] && ft_strcmp(args[i], "-n") == 0)
        i++;
	new_line = i;
	while (args[i])
	{
    	printf("%s", args[i]);
		i++;
		if(args[i])
			printf(" ");
	}
    if (new_line == 1 || !(args[1]))
        printf("\n");
	fflush(stdout);
    return (0);
}
//-------------------------------------------------------

//////////////////////////// pwd ////////////////////////////
int		pwd()
{
	char cwd[128];
	glob_vars.exitstatus = 0;
	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
	{
		// get env variable(PWD), and print it
		printf("PWD");
	}
	return (0);
}
