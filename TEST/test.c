#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv, char **env)
{
	char *c = malloc(3);
	c[0] = 1;
	c[1] = 'l';
	c[2] = 0;

	char *arg[] = {"rm", c, 0};
	printf("called\n");
}
