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
	int len = 4096;
	void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0 );
	if(ptr == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}

	//create child process
	
	pid_t pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(-1);
	}
	if(pid > 0)
	{
		strcpy((char *)ptr, "success copy");
		wait(NULL);
	}
	if(pid == 0)
	{
		printf("%s\n",(char *)ptr);
	}
	
	munmap(ptr,len);
	return 0;
}
