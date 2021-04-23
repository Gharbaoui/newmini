#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

char *decimal_to_binary(int n);

int main(){
    int pid;
    int status;

    
    pid = fork();
    if (pid == 0)
    {
        char *cat[] = {"cat", "f", NULL};
        int ret;
        ret = execve("/usr/bin/cat", cat, NULL);
    }

    waitpid(pid, &status, 0);
    printf("%d\n", WEXITSTATUS(status));
}


char *decimal_to_binary(int n)
{
  int c, d, t;
  char *p;

  t = 0;
  p = (char*)malloc(16+1);

  if (p == NULL)
    exit(EXIT_FAILURE);

  for (c = 15 ; c >= 0 ; c--)
  {
    d = n >> c;

    if (d & 1)
      *(p+t) = 1 + '0';
    else
      *(p+t) = 0 + '0';

    t++;
  }
  *(p+t) = '\0';

  return  p;
}

