#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int n = 0, flag = 0;

void do_sig_child(int signo)
{
	printf("I am child  %d,%d\n",getpid(),n);
	n += 2;
	flag = 1;
	//sleep(1);
}

void do_sig_parent(int signo)
{
	printf("I am parent %d,%d\n",getpid(),n);
	n += 2;
	flag = 1;
	//sleep(1);
}

int main()
{
	pid_t pid;
	struct sigaction act;
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	if(pid >0)
	{
		n = 2;
		sleep(1);
		act.sa_handler = do_sig_parent;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR2, &act, NULL);

		flag = 1;
		while(1)
			if(flag == 1)
			{
				kill(pid,SIGUSR1);
				//-----------------lost cup
				flag = 0;
			}
		
	}
	else if(pid == 0)
	{
		n = 1;
		act.sa_handler = do_sig_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR1, &act, NULL);

		while(1)
			if(flag == 1)
			{
				kill(getppid(),SIGUSR2);
				flag = 0;
			}
		
	}

	return 0;
}
