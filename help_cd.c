#include "minishell.h"

int	help_cd(char **paths, t_fullvar **vars, char *home)
{
	int	ret;

	if (paths[1] == NULL && home == NULL)
	{
		dup2(2, 1);
		ft_printf(1, "Minishell: cd: HOME not set\n");
		dup2(1, glob_vars.fdout);
		glob_vars.exitstatus = 1;
		return (1);
	}
	else if (paths[1])
		home = paths[1];
	else if (home[0] == '\0')
		home = ".";
	ret = chdir(home);
	if (ret)
	{
		glob_vars.exitstatus = 1;
		perror("minishell: cd");
		return (ret);
	}
	set_pwd(vars);
	update_pwd((*vars)->exenvs);
	return (0);
}

int	cd(char **paths, t_fullvar **vars)
{
	int		i;
	t_envs	*var;
	char	*home;

	home = NULL;
	glob_vars.exitstatus = 0;
	if (paths[1])
		home = paths[1];
	else
	{
		var = get_env(&i, "HOME", (*vars)->exenvs);
		if (i)
			home = var->env_value;
	}
	return (help_cd(paths, vars, home));
}

void	change_pwd_old_pwd(t_fullvar **vars, char *opwd)
{
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
