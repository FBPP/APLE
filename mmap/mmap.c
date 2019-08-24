#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int fd = open("Text.txt", O_RDWR);
	int len = lseek(fd, 0, SEEK_END);
	void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
	if(ptr == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	printf("%s", (char *)ptr);

	munmap(ptr,len);
	close(fd);
	return 0;
}
