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

class Phier{
	private:
		Semaphore mutex[N];
		pthread_t thread[N];
		int num;
	public :
		Phier():num(0)
		{
			thread_create();
			thread_wait();
		}
	private:
		void thread_create(){
			int tmp;
			for(int i = 0; i < N; i++)
			{
				tmp = pthread_create(&thread[i], NULL, solve, this);
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
			Phier * _this = (Phier *) arg;
			_this->run();
		}
		void run ()
		{
			int left = num;
			int right = (num + 1) % N;
				if(num % 2 == 0)
				{
					 mutex[left].P();
					 mutex[right].P();
					 printf("phi %d eat\n", ++num);
					 mutex[right].V();
					 mutex[left].V();
				}
				else
				{
					 mutex[right].P();
					 mutex[left].P();
					 printf("phi %d eat\n", ++num);
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
