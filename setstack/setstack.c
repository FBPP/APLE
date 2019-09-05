#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 0x10000000

void * fun(void * arg){
	while(1);
	return NULL;
}

int main(){
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	int i = 0;
	while(1)
	{
		void * staddr = malloc(SIZE);
		if(staddr == NULL)
		{
			perror("malloc error");
			exit(1);
		}

		pthread_attr_setstack(&attr, staddr, SIZE);
		int err = pthread_create(&tid, &attr, fun, NULL);
		if(err)
		{
			printf("%s\n", strerror(err));
			exit(1);
		}
		printf("%d\n", ++i);
	}

	pthread_attr_destroy(&attr);

	return 0;
}
