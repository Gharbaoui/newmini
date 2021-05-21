#include "Minishell.h"

int	cd(char **paths, t_fullvar **vars)
{
	int		i;
	t_envs	*var;
	char	*home;

	if (h1_cd(&home, paths, vars))
		return (1);
	if (paths[1] == NULL && home == NULL)
	{
		printf("Minishell: cd: HOME not set\n");
		glob_vars.exitstatus = 1;
		return (1);
	}
	else if (paths[1])
		home = paths[1];
	else if (home[0] == '\0')
		home = ".";
	if (h2_cd(home))
		return (1);
	set_pwd(vars);
	return (i);
}

void	change_pwd_old_pwd(t_fullvar **vars, char *opwd)
{
	t_envs	*var;
	char	*help;
	char	**oldpwd_newpwd;
	char	*path;

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

void	set_pwd(t_fullvar **vars)
{
	int		found;
	t_envs	*var;

	var = get_env(&found, "PWD", (*vars)->exenvs);
	if (found)
		change_pwd_old_pwd(vars, var->env_value);
	else
	{
		unset_one_var("OLDPWD", vars);
		sub_export(vars, "OLDPWD");
	}
}

int	ft_env (t_fullvar *vars)
{
	t_envs		*var;
	struct stat	fst;
	t_words		*help;
	char		*envpath;
	int			found;

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
	return (0);
}

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
