#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count;
pthread_rwlock_t rwlock;

void * th_write(void * arg){
	int t;
	int i = (int)arg;
	while(1)
	{
		t = count;
		usleep(1000);

		pthread_rwlock_wrlock(&rwlock);
		printf("==========write %d: %lu: count = %d ++count  =%d\n ", i, pthread_self(),t, ++count);
		pthread_rwlock_unlock(&rwlock);
		usleep(5000);
	}
}

void * th_read(void * arg){
	int i = (int) arg;
	while(1)
	{
	 	pthread_rwlock_rdlock(&rwlock);
		printf("----------read %d: %lu: count = %d\n", i, pthread_self(), count);
		pthread_rwlock_unlock(&rwlock);
		usleep(900);
	}
}

int main()
{
	int i;
	pthread_t tid[8];
	pthread_rwlock_init(&rwlock,NULL);

	for(i = 0; i < 8; i++)
	{	if(i  < 3)
			pthread_create(&tid[i], NULL, th_write, (void *)i);
		else 
			pthread_create(&tid[i], NULL, th_read, (void *)i);
	}

	for(i = 0; i < 8; i++)
		pthread_join(tid[i], NULL);

	pthread_rwlock_destroy(&rwlock);


	return 0;
}
