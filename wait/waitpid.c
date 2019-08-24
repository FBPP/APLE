#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int n = 5, i;
	for(i = 0; i < 5; i++)
	{
		pid_t pid = fork();
		if(pid == 0)
			break;
		else if(pid == -1)
		{
			perror("fork error");
			exit(1);
		}
	}

	if(n == i)
	{
		sleep(n);
		printf("i am parrent\n");
		int wid;
		do 
		{
			 wid = waitpid(-1, NULL, 0);
		} 
		while(wid != -1 || wid == 0);
		while(1);
	}
	else
	{
		sleep(i);
		printf("i am %dth child, pid = %d, gpid = %d\n", i + 1, getpid(), getppid());
	}
	return 0;

}
