#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char * argv[])
{

	if(argc < 2)
	{
		perror("./a.out fifoname");
		exit(1);
	}
	if(access(argv[1],F_OK) == -1) // argc[1] is not exist
	{
		int r = mkfifo(argv[1],0664);
		if(r == -1)
		{
			perror("mkfifo error");
			exit(1);
		}
		printf("%s fifo is created \n",argv[1]);
	}
	char buf[1024] = {0};
	int fd = open(argv[1],O_RDONLY);
        int length =  read(fd,buf,sizeof(buf));
	for(int i = 0; i < 100; i++)
	{
		printf("%d ,",buf[i]);
	}
	close(fd);		

	return 0;
}
