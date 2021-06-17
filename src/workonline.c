#include "minishell.h"

int	workon_line(char *line, t_completecmd **complete, int numofcmds, int help)
{
	t_words			*commands;
	t_workingcmds	*wcmd;

	*complete = NULL;
	help = workon_line_part1(line, &commands, &wcmd, &numofcmds);
	if (help != SUCCESS)
		return (help);
	help = fill_completecmd(complete, wcmd->cmds, 0);
	if (help != SUCCESS)
	{
		workon_line_part2(&wcmd, &commands, numofcmds, complete);
		return (help);
	}
	help = filter_complete(complete);
	if (help != SUCCESS)
	{
		*complete = NULL;
		free_comp(complete);
		return (help);
	}
	free_wcmd(&wcmd, numofcmds);
	free_words(&commands);
	return (SUCCESS);
}

int	workon_line_part1(char *line, t_words **commands,
	t_workingcmds **wcmd, int *numofcmds)
{
	int	help;

	help = check_newlines(line);
	if (help != SUCCESS)
		return (PARSERROR);
	rest(wcmd, commands);
	help = fill_commands(commands, line);
	if (check_errors(help, commands))
		return (help);
	*numofcmds = countnumberofcmds(*commands);
	if (*numofcmds <= 0)
	{
		free_words(commands);
		return (*numofcmds);
	}
	help = fill_wcmd(wcmd, *commands, *numofcmds);
	if (help != SUCCESS)
	{
		free_words(commands);
		free_wcmd(wcmd, *numofcmds);
		return (help);
	}
	return (SUCCESS);
}

int	workon_line_part2(t_workingcmds **wcmd, t_words **commands,
	int numofcmds, t_completecmd **complete)
{
	free_words(commands);
	free_wcmd(wcmd, numofcmds);
	free_comp(complete);
	return (0);
}
