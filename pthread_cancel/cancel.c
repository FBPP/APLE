#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void * arg){
	while(1)
	{
	//	printf("fun()\n");
	//	sleep(1);
		pthread_testcancel();
	}
	return NULL;
}


int main(){
	pthread_t tid;
	pthread_create(&tid, NULL, fun, NULL);
	sleep(3);
	pthread_cancel(tid);
	int err;
        pthread_join(tid, &err);
	printf("%d\n", err);
	return 0;
}
