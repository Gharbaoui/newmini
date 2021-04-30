#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>


void func(int *i)
{
	srand(time(NULL));   // Initialization, should only be called once.
	int r = rand();  
	printf("%d\n", r); 
	*i = -1;
}
int main()
{
	int i;

	i = 0;
	i += 90;
	func(&i);
	int pid = fork();
	if (pid == 0)
	{
		abort();
	}
	wait(NULL);
	printf("Pranrt\n");
}
