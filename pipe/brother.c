#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	int fd[2];
	int ret = pipe(fd);
	if(ret == -1)
	{
		perror("pipe error");
		exit(1);
	}
	int i = 0;
	while(i<2)
	{	
		printf("%d\n",i);	
		pid_t pid = fork();
		if(pid == 0) break;	
		if(pid == -1)
		{
			perror("pipe error:");
			exit(1);
		}
		i++;
	}


	if(i == 0)
	{
		close(fd[1]);
		dup2(fd[0],0);
		execlp("grep", "ps","push","--color=auto",NULL);

	}
	if( i == 1)
	{
		close(fd[0]);
		dup2(fd[1],1);
		execlp("ps","ps","aux",NULL);

	}
	if(i == 2)
	{
		close(fd[0]);
		close(fd[1]);
		pid_t wpid;
		while((wpid = waitpid(-1, NULL, WNOHANG)) != -1)
		{
			if(wpid != 0 ) printf("pid = %d\n",wpid);
		}
	}
	return 0;
}
