#include <stdio.h>
#include <stdlib.h>

void test(int size)
{
	char *str = malloc(size);
	free(str);
}


int main(){
	int i = 0;
	while (++i <= 10)
		test(i);
}
