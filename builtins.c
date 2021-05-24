#include "minishell.h"


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
	struct stat fst;
	t_words *help;
	char *envpath;
	int found;

	help = vars->filledvar;
	glob_vars.exitstatus = 0;
	while (help)
	{
		var = get_env(&found, help->txt, vars->exenvs);
		printf("%s=%s\n", var->env_name, var->env_value);
		help = help->next;
	}
	var = get_env(&found, "PATH", vars->exenvs);
	if (found)
		envpath = get_using_path("/env", var->env_value, &fst);
	printf("_=%s\n", envpath);
	free(envpath);
	return 0;
}
//////



////// exit

int is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

int is_number(char *num)
{
	if (*num == '-' || *num == '+')
		num++;
	while (*num)
	{
		if (!is_digit(*num))
			return 0;
		num++;
	}
	return 1;
}

int ft_exit(char **args)
{
	int status;
	glob_vars.exit = 1;
	if (args[1])
	{
		if (is_number(args[1]) == 0)
		{
			printf("bash: exit: %s: numeric argument required\n", args[1]);
			glob_vars.exitstatus = 2;
		}else if (args[2])
		{
			printf("bash: exit: too many arguments\n");
			glob_vars.exitstatus = 1;
			glob_vars.exit = 0;
		}else{
			status = ft_atoi(args[1]) & 255;    ///// atoi to ft_atoi here
			glob_vars.exitstatus = status;
		}
	}
	return glob_vars.exitstatus;
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
