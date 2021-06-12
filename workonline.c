#include "minishell.h"

int workon_line(char *line, t_completecmd **complete, int numofcmds, int help)
{
	int last, is_str;
	t_words *commands;
	t_workingcmds *wcmd;

	*complete = NULL;
	help = workon_line_part1(line, &commands, &wcmd, &numofcmds);
	if (help != SUCCESS)
		return (help);
	help = fill_completecmd(complete, wcmd->cmds, 0);
	if (help != SUCCESS)
	{
		/*free_words(&commands);
		free_wcmd(&wcmd, numofcmds);
		free_comp(complete);*/
		workon_line_part2(&wcmd, &commands, numofcmds, complete);
		return help;
	}
	help = filter_complete(complete);
	if (help != SUCCESS)
	{
		*complete = NULL;
		free_comp(complete);
		return help;
	}
	free_wcmd(&wcmd, numofcmds);
	free_words(&commands);
	return SUCCESS;
}
