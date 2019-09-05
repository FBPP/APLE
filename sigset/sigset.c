#include <stdio.h>
#include <signal.h>

int main()
{
	sigset_t myset, oldset, pend;
	sigemptyset(&myset);
	sigaddset(&myset, SIGQUIT);

	sigprocmask(SIG_BLOCK, &myset, &oldset);
	
	while(1)
	{
		sigpending(&pend);
		for(int i = 1; i <= 32; i++)
		{
			if(sigismember(&pend, i) == 1)
				putchar('1');
			else  putchar('0');
		}
		printf("\n");
		sleep(1);
	}

	return 0;
}
