#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv, char **env)
{
	int fd;
	fd = open("filename", O_WRONLY | O_APPEND | O_CREAT, 0644);
	dup2(fd, 1);
	printf("hello   world\n");
}
