#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
	int pid;

	scanf("%d", &pid);
	kill (pid, SIGINT);
}
