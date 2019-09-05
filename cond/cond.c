#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>

struct  msg{
	int num;
	struct msg * next;
};
struct msg * head = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;

void * producter(void * arg){
	struct msg * mp;
	while(1)
	{
		mp = malloc(sizeof(struct msg));
		mp->num = rand() % 400 + 1;
		printf("------producted  %d\n", mp->num);
		pthread_mutex_lock(&mutex);
		mp->next = head;
		head = mp;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&has_product);
	}
	return NULL;
}
void * consumer(void * arg){

	struct msg  * mp;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while (head == NULL)
			pthread_cond_wait(&has_product, &mutex);
		mp = head;
		head = mp ->next;
		pthread_mutex_unlock(&mutex);
		printf("=======consumer %d, %d\n", (int)arg, mp->num);
		free(mp);
		mp =  NULL;
	}


	return NULL;
}
int main(){
	srand(time(NULL));
	pthread_t protid, contid1, contid2;

	pthread_create(&protid, NULL, producter, NULL);
	pthread_create(&contid1, NULL, consumer, (void * ) 1);
//	pthread_create(&contid2, NULL, consumer, (void * ) 2);

	pthread_join(protid, NULL);
	pthread_join(contid1, NULL);
//	pthread_join(contid2, NULL);

	return 0;
}
