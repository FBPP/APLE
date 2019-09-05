#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
	char ch;
	int var;
	char str[64];
} Exit_t;

void * thrd_func(void * arg){
	printf("thread id = %lu", pthread_self());
	Exit_t * p = (Exit_t *) malloc(sizeof(Exit_t));

	p->ch = 'a', p->var = 1, strcpy(p->str,"my thread\n");

	pthread_exit(p);
}
int main(){
	pthread_t tid;
	Exit_t * p;
       	pthread_create(&tid, NULL, thrd_func, NULL);
	pthread_join(tid, (void**)&p);
	printf("ch = %c, var = %d, str = %s\n", p->ch, p->var, p->str);
	free(p);

	

	return 0;
}
