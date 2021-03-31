#include "exec.h"

int main(int argc, char **argv, char **env)
{
    int pid;
    pid = fork();
    if (pid == 0)
    {
        char *ex[] = {"./ask", 0};
        execve("moh", ex, env);
        return 1;
    }
    wait(NULL);
    

}
