#include "minishell.h"


void print_cmd(t_cmd *cmd, int level)
{
	printspaces(level);
	printf ("command -> : %s|\n", cmd->command);
	printf("================\n");
	print_oldwords(cmd->txts, level, "txt");
	printf("================\n");
	print_oldwords(cmd->files, level, "file");
	printf("================\n");
	print_oldwords(cmd->ops, level, "op");

}

void print_oldwords(t_words *word, int level, char *name)
{
	if (word)
	{
		printspaces(level);
		printf("%s -> : %s|\n", name, word->txt);
		print_oldwords(word->next, level + 1, name);
	}
}



void print_pipcmd(t_pipcmd *pipcmd)
{
	if (pipcmd)
	{
		print_cmd(pipcmd->cmd, 0);
		printf("\033[0;33m");
		printf("----------------------------------------------------------\n");
		printf("\033[0m");
		print_pipcmd(pipcmd->next);
	}
}

void print_completecmd(t_completecmd *complete)
{
	if (complete)
	{
		print_pipcmd(complete->splcommand);
		printf("\033[0;32m");
		printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("\033[0m");
		print_completecmd(complete->next);
	}
}

