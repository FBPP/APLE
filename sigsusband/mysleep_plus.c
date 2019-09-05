#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void catch(int signo)
{
	;
}
unsigned int mysleep(unsigned int  seconds)
{
	struct sigaction act, oldact;
	act.sa_handler = catch;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	int ret = sigaction(SIGALRM, &act, &oldact);
	if(ret == -1 )
	{
		perror("sigcation error");
		exit(-1);
	}

	sigset_t newmask,oldmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	alarm(seconds);
	
	sigset_t suspmask = oldmask;
	sigdelset(&suspmask,SIGALRM);
	
	sigsuspend(&suspmask);

	ret = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);
	sigprocmask(SIG_SETMASK,&oldmask, NULL);

	return ret;
}
int main()
{
	while(1)
	{
		mysleep(3);
		printf("-----------------\n");
	}
	return 0;
}
