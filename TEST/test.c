#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv, char **env)
{
	int d;
	char *f = malloc(10);
	d = 10;
	do {
		d--;
		write(1,"A",1);
		read(0, f, 4);
	}	while (d);
}
