#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 6

using namespace std;

class Semaphore{
	private:
		sem_t sem;
	public:
		Semaphore(int value = 1)
		{
			sem_init(&sem, 0, value);
		}
		void P()
		{
			sem_wait(&sem);
		}
		void V()
		{
			sem_post(&sem);
		}
		~Semaphore()
		{
			sem_destroy(&sem);
		}
};

class Phier;
struct wrapper{
	int i;
	Phier * p;
};

class Phier{
	private:
		Semaphore mutex[N];
		pthread_t thread[N];
		wrapper wp[N];
	public :
		Phier()
		{
			thread_create();
			thread_wait();
		}
	private:
		void thread_create(){
			int tmp;
			for(int i = 0; i < N; i++)
			{
				wp[i].i = i, wp[i].p = this;
				tmp = pthread_create(&thread[i], NULL, solve, &wp[i]);
				if(tmp)
					printf("th %d create fail\n", i);
			}
		}

		void thread_wait(){
			for(int i = 0; i < N; i++)
			{
				pthread_join(thread[i], NULL);
				printf("th %d end\n", i);
			}

		}
		static void * solve(void * arg){
			wrapper * wp = (wrapper *)arg;
			wp->p->run(wp->i);
		}
		void run (int i)
		{
			int left = i;
			int right = (i + 1) % N;
				if(i % 2 == 0)
				{
					 mutex[left].P();
					 mutex[right].P();
					 printf("phi %d eat\n", i);
					 mutex[right].V();
					 mutex[left].V();
				}
				else
				{
					 mutex[right].P();
					 mutex[left].P();
					 printf("phi %d eat\n", i);
					 mutex[left].V();
					 mutex[right].V();
				}
				sleep(1);

			pthread_exit(NULL);

		}
};
int main(){
	Phier p;
	return 0;
}
