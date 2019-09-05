#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void * fun(void * arg){
	while(1)
	{
		srand(time(NULL));
		printf("hello");
		sleep(rand() % 3);
		printf("world\n");
		sleep(rand() % 3);
	}
	return NULL;
}

int main(){
	pthread_t tid;
	srand(time(NULL));

	pthread_create(&tid, NULL, fun, NULL);
	while(1)
	{
		printf("HELLO");
		sleep(rand() % 3);
		printf("WROLD\n");
		sleep(rand() % 3);
	}

	return 0;
}
