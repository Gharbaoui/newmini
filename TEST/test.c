#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(){
    int fd = open ("txt", O_WRONLY);
    int copy = dup(1);
    dup2(fd, 1);
    printf("HELLO");
    fflush(stdout);
    dup2(copy, 1);
    printf("OUT\n"); 
}
