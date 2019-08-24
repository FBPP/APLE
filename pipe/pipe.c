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
		char buf[1024];
		int size = read(fd[0], buf, sizeof(buf));
		if(size == 0)
		{
			printf("-------\n");
		}
			write(STDOUT_FILENO, buf, size);

	}
	else
	{
		close(fd[0]);
		char * str = "hello pipe\n";
		write(fd[1],str, strlen(str));
		sleep(1);
	}
	return 0;
}
