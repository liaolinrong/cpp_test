#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

void* thread1(void* arg)
{
	for(int i = 0; i < 10; i++)
	{
		pthread_mutex_lock(&mutex1);
		printf("thread1 %d\n",i);
		sleep(1);
		pthread_mutex_lock(&mutex2);
		printf("thread1 after %d\n",i);
		sleep(1);
		pthread_mutex_unlock(&mutex2);
		pthread_mutex_unlock(&mutex1);
		sleep(1);
	}
}

void* thread2(void* arg)
{
        for(int i = 0; i < 10; i++)
        {
		pthread_mutex_lock(&mutex2);
                printf("thread2 %d\n",i);
		sleep(1);
		pthread_mutex_lock(&mutex1);
		printf("thread2 after %d\n",i);
                sleep(1);
		pthread_mutex_unlock(&mutex2);
                pthread_mutex_unlock(&mutex1);
		sleep(1);
        }
}

int main()
{
	pthread_t t1,t2;
	
	pthread_create(&t1,NULL,thread1,NULL);
	pthread_create(&t2,NULL,thread2,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}
