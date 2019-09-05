#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void* func(void* arg){
	sleep(1);
}

int main(){
	pthread_t tid;
	pthread_create(&tid, NULL, func, NULL);
	pthread_detach(tid);
	int err = pthread_join(tid, NULL);
	printf("%d\n", err);
	fprintf(stderr,"%s\n", strerror(err));

	return 0;
}
