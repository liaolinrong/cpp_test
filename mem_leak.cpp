#include <stdio.h>
#include <stdlib.h>

int main()
{
	char* p = (char*)malloc(1024);
	
	printf("%p\n", p);
	//free(p);
	return 0;
}
