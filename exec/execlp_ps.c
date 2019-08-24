#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd;
	fd = open("ps.out", O_RDWR | O_CREAT | O_TRUNC , 0664);
	if(fd < 0) exit(1);
	dup2(fd, STDOUT_FILENO);
	execlp("ps", "ps", "aux", NULL);

	return 0;
}
