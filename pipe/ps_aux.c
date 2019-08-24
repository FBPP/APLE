#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int fd[2];
	int ret = pipe(fd);
	if(ret == -1)
	{
		perror("pipe error");
		exit(1);
	}

	pid_t pid = fork();
	if(pid == -1)
	{
		perror("pipe error:");
		exit(1);
	}
	else if(pid == 0)
	{
		close(fd[1]);
		dup2(fd[0],0);
		execlp("grep", "ps","push","--color=auto",NULL);

	}
	else
	{
		close(fd[0]);
		dup2(fd[1],1);
		execlp("ps","ps","aux",NULL);

	}
	return 0;
}
