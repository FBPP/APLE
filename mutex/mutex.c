#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex;

void * fun(void * arg){
	while(1)
	{
		pthread_mutex_lock(&mutex);
		srand(time(NULL));
		printf("hello");
		sleep(rand() % 3);
		printf("world\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
	}
	return NULL;
}

int main(){
	pthread_t tid;
	srand(time(NULL));

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&tid, NULL, fun, NULL);
	while(1)
	{	pthread_mutex_lock(&mutex);
		printf("HELLO");
		sleep(rand() % 3);
		printf("WROLD\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
