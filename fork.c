#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	printf("xxxxxxxx\n");
	int i;
	for(i = 0; i< 5; i++)
	{
		pid = fork();
		if(pid == -1)
		{
			perror("fork error");
			exit(-1);
	
		}	
		else if( pid == 0)
		{
			sleep(i);
			printf("i am %d child , pid = %u\n", i+1, getpid());
			break;
		}
		
	}
	if(pid >0)
	{
		sleep(i);
	}
	return 0;
}
