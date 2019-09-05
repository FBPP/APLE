#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void * func(void * arg){
	int i  = (int) arg;
	printf("%dth thread id = %lu\n", i, pthread_self());
	return NULL;
}

int main()
{
	pthread_t tid;

	int i;
	for(i = 0; i < 5; i++)
	{
		int ret = pthread_create(&tid, NULL, func, (void *)i);
		if(ret != 0)
		{
			fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
			exit(1);

		}
	}
	printf("main:%lu\n", pthread_self());
	pthread_exit(NULL);
}
