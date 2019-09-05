#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void * fun(void * arg){
	while(1);
	return NULL;
}

int main(){
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&tid, &attr, fun, NULL);

	int ret = pthread_join(tid,NULL);
	printf("%d\n", ret);

	pthread_attr_destroy(&attr);

	return 0;
}
