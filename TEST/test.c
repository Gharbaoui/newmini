#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(){
    int fd = open ("txt", O_WRONLY);
    dup2(fd, 1);
    printf("Hello \n");
}
