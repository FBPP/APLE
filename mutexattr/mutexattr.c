#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct mt{
	int num;
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutexattr;
};

int main(){
	struct mt * mm;
	mm = mmap(NULL, sizeof(struct mt), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
	memset(mm, 0, sizeof(struct mt));

	pthread_mutexattr_init(&mm->mutexattr);
	pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);

	pthread_mutex_init(&mm->mutex, &mm->mutexattr);
	
	pid_t pid = fork();
	if(pid == 0)
	{
		for(int i = 0; i < 10; i++)
		{
			pthread_mutex_lock(&mm->mutex);
			(mm->num)++;
			printf("---child---num++ %d\n", mm->num);
			pthread_mutex_unlock(&mm->mutex);
			sleep(1);
		}
	}
	if(pid > 0)
	{
		for(int i = 0; i < 10; i++)
		{
			sleep(1);
			pthread_mutex_lock(&mm->mutex);
			mm->num += 2;
			printf("-----partent----num+=2 %d\n", mm->num);
			pthread_mutex_unlock(&mm->mutex);
		}
		wait(NULL);
	}
	pthread_mutexattr_destroy(&mm->mutexattr);
	pthread_mutex_destroy(&mm->mutex);

	return 0;
}
