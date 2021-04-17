#include <unistd.h>
#include <stdio.h>
       #include <sys/types.h>
       #include <sys/wait.h>

int main(int argc, char **argv, char **env)
{
    int fd[2];
    pipe(fd);
    int pid;
    
    pid = fork();

    if (pid == 0)
    {
        dup2(f, 1);
        execve("/usr/bin/ls", argv, env);
    }
    close(fd[1]);
    wait(NULL);
    dup2(fd[0], 0);
    char *argv1[] = {"tes", "t", NULL};
    execve("/usr/bin/grep", argv1, env);
}
