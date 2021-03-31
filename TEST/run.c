#include <stdio.h>
#include <unistd.h>
int main()
{
    int x;
    read(0, &x, sizeof(int));
}
