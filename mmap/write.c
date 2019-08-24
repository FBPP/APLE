#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
	int fd = open("Temp", O_RDWR | O_CREAT, 0664 );
	const int len = 4096;
	ftruncate(fd, len);
	void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
	if(ptr == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	char str[] = {"hello 1"};
	while(1)
	{
		strcpy(ptr,str);
		str[6]++;
		sleep(1);
	}
	munmap(ptr,len);
	close(fd);
	return 0;
}
