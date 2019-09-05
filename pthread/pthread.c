#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void * func(void * arg){
	printf("thread id = %lu\n", pthread_self());
	return NULL;
}

int main()
{
	pthread_t tid;

	int ret = pthread_create(&tid, NULL, func, NULL);
	if(ret != 0)
	{
		fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
		exit(1);

	}
	printf("main:%lu\n", pthread_self());
	sleep(1);

	return 0;
}
