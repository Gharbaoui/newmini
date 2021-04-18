#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv, char **env)
{
	int pid;
	int fd[2];

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		char *ar[] = {"cat", "file", NULL};
		execve("/bin/cat", ar, NULL);
	}
	
	pid = fork();
	if (pid == 0)
	{
		char *g[] = {"cat", "-e", NULL};
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execve("/bin/cat", g, NULL);
	}

	wait(NULL);
	wait(NULL);
}
