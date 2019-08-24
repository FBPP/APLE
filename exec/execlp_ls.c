#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{

	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	}else if(pid > 0)
	{
		printf("parrent\n");
		sleep(1);
	}
	else
	{
		execlp("ls", "ls", "-l", "-a", NULL);
	}
	return 0;
}
