#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PROCESS_NO 4
#define SOURCE "source.txt"
int main()
{
	int fd_source;
	if(!access(SOURCE,F_OK))
	{
		fd_source = open(SOURCE,O_RDONLY);		
		if(fd_source == -1)
		{
			perror("error open:source file is not exist");
			exit(1);
		}
	}
	int len = lseek(fd_source, 0, SEEK_END);
	void * ptr_source = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd_source, 0 );
	if(ptr_source == MAP_FAILED)
	{
		perror("mmap error source");
		exit(1);
	}

	int fd_copy = open("CopyFile.txt",O_RDWR | O_CREAT,0664);
	if(fd_copy == -1)
	{
		perror("error open CopyFile.txt");
		exit(1);
	}
	if(ftruncate(fd_copy, len) == -1)
	{
		perror("ftruncate error");
		exit(1);
	}	
	void * ptr_copy = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_copy, 0 );
	if(ptr_copy == MAP_FAILED)
	{
		perror("mmap error copy");
		exit(1);
	}

	int i = 0;
	for(i; i < PROCESS_NO; i++)
	{
		pid_t pid = fork();
		if(pid == 0)
			break;
		if(pid == -1)
		{
			perror("fork  error");
			exit(1);
		}

	}

	if(i < PROCESS_NO)
		memcpy(ptr_copy +  i * len/PROCESS_NO, ptr_source + i * len/PROCESS_NO,len/PROCESS_NO );
	
	if(i == PROCESS_NO)
	{	
		memcpy(ptr_copy + i * len/PROCESS_NO, ptr_source + i * len/PROCESS_NO, len%(len/PROCESS_NO));	
		while(wait(NULL) != -1);
		munmap(ptr_source,len);
		munmap(ptr_copy,len);
		close(fd_source);
		close(fd_copy);
	}
	return 0;
}
