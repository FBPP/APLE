#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	pid_t pid, sid;
	pid = fork();
	if(pid > 0)
		return 0;

	sid = setsid();

	int ret =  chdir("/home/pengpeng");
	if(ret < 0)
	{
		perror("chdir error");
		exit(1);
	}

	umask(0002);

	close(0);
	int fd = open("/dev/null",O_RDWR);
	dup2(0, STDOUT_FILENO), dup2(0, STDERR_FILENO);

	while(1);

	return 0;
}
