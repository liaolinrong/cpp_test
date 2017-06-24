#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
void sig_handler(int sig)
{
	if (sig == SIGINT){
		printf("aaa get SIGINT, exit");
		exit(0);
	}
}

int main()
{
	signal(SIGINT, sig_handler);

	while(1){
		printf("hello\n");
		sleep(1);
	}

	return 0;
}
