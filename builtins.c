#include "./minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>




//////////////////////////// CD ////////////////////////////
int cd(char *path)
{
	int i;
	char *home;

	if (path == NULL && (home = getenv("HOME")) == NULL) // TODO: to change getenv to the builtin export
	{
		printf("Minishell: cd: HOME not set\n");
		return (1);
	}
	else
		home = path;
	i = chdir(home);
	if (i)
		perror("minishell: cd");
		// printf("minishell: cd: %s: No such file or directory\n", path);

	// TODO: set env variables PWD and OLDPWD
	return (i);
}
//-------------------------------------------------------


//////////////////////////// echo ////////////////////////////
int     _echo(char **args)
{
    int new_line;
	int i;

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

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
	{
		// get env variable(PWD), and print it
		printf("PWD");
	}
	return (0);
}
