#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

void func(int signo)
{
	printf("hello\n");
}

int main()
{
	struct itimerval new = {{3,0},{5,0}}, old;
	signal(SIGALRM,func);
	
//	new.it_value.tv_sec = 5;
//	new.it_value.tv_usec = 0
	setitimer(ITIMER_REAL, &new, &old);
	while(1);
	
	return 0;
}
