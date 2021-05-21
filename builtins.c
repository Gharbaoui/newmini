#include "minishell.h"

int	is_number(char *num)
{
	if (*num == '-' || *num == '+')
		num++;
	while (*num)
	{
		if (!is_digit(*num))
			return (0);
		num++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	int	status;

	glob_vars.exit = 1;
	if (args[1])
	{
		if (is_number(args[1]) == 0)
		{
			printf("bash: exit: %s: numeric argument required\n", args[1]);
			glob_vars.exitstatus = 2;
		}
		else if (args[2])
		{
			printf("bash: exit: too many arguments\n");
			glob_vars.exitstatus = 1;
			glob_vars.exit = 0;
		}
		else
		{
			status = ft_atoi(args[1]) & 255;
			glob_vars.exitstatus = status;
		}
	}
	return (glob_vars.exitstatus);
}

int	_echo(char **args)
{
	int	new_line;
	int	i;

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
		if (args[i])
			printf(" ");
	}
	if (new_line == 1 || !(args[1]))
		printf("\n");
	return (0);
}

int	pwd(void)
{
	char	cwd[128];

	glob_vars.exitstatus = 0;
	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		printf("PWD");
	return (0);
}
