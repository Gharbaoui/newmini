#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv, char **env)
{
    int fd[2];
    pipe(fd);
    char *cat[] = {"cat", NULL};
    char *ls[] = {"ls",  NULL};
    int pid;

    pid = fork();
    if (pid == 0)
    {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execve("/usr/bin/cat", cat, NULL);
    }

    pid = fork();
    if (pid == 0)
    {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execve("/usr/bin/ls", ls, NULL);
    }

    close(fd[0]);
    //close(fd[1]);
    wait(NULL);
    wait(NULL);
    
}
