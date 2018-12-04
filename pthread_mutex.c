//pthread_mutex.c
#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <pthread.h>

int sum=0;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

void *do_sum(void *val)
{
	int	i;
	int	n = *((int *)val);
	for(i = 0; i < n; i++)
	{
		pthread_mutex_lock(&mutex);
        	printf("[%x]", pthread_self()); //pthread_self() : 자신의 thread id 리턴
		printf("sum : %d\n", sum);
		sum ++;
		pthread_mutex_unlock(&mutex);
	}
}
int main( )
{
	int	status;
	pthread_t p_thread[2];
	int	a=5 ;
	int 	b=10;
	status = pthread_create(&p_thread[0], NULL, do_sum, (void *) &a);
	status = pthread_create(&p_thread[1], NULL, do_sum, (void *) &b);
	pthread_join(p_thread[0], (void **) &status);
	pthread_join(p_thread[1], (void **) &status);
        pthread_mutex_destroy(&mutex);
	printf("programing is end");
	return 0;
}
