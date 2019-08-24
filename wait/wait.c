#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid, wpid;
	pid = fork();
	if(pid == 0)
	{
		printf("---child,my parent =%d, going to sleep 3s \n",getppid());
		sleep(3);
		printf("----------------child die-----------------\n");
		exit(76);
	}
	else if(pid > 0)
	{
		int status;
		wpid = wait(&status);
		if(wpid == -1)
		{
			perror("wait error");
			exit(1);
		}
		if(WIFEXITED(status))
		{
			printf("child exit with %d\n", WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status))
		{
			printf("cjild exit with %d\n", WTERMSIG(status));
		}
		while(1)
		{
			printf("i am parrent,pid = %d, myson = %d\n", getpid(),pid);
			sleep(1);
		}
	}
	return 0;
}
