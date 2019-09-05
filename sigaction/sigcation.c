#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void catch(int signo)
{
	printf("%d signal is catched\n", signo);
	sleep(10);
	printf("---------finish----------\n");
}

int main()
{
	struct sigaction act;
	act.sa_handler = catch;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);
	act.sa_flags = 0;
	
	int  ret = sigaction(SIGINT, &act, NULL);
	if(ret <0)
	{
		perror("sigaction  error");
		exit(-1);
	}
	while(1);
	return 0;
}
