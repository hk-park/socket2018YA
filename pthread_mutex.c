#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int sum = 0;

void *do_sum(void *val);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
	int status;
	pthread_t thread_id[3];
	int a = 2;
	int b = 10;
	int c = -1;
	status = pthread_create(&thread_id[0], NULL, do_sum, (void*)&a);
	status = pthread_create(&thread_id[1], NULL, do_sum, (void*)&b);
	status = pthread_create(&thread_id[2], NULL, do_sum, (void*)&c);
	pthread_join(thread_id[0], (void **)&status);
	pthread_join(thread_id[1], (void **)&status);
	pthread_join(thread_id[2], (void **)&status);
	printf("main program ends\n");
	pthread_mutex_destroy(&mutex);
	return 0;
}


void *do_sum(void *val){
	int i;
	int n = *((int *)val);
	for(i=0; i<10; i++){
		pthread_mutex_lock(&mutex);
		sum+=n;
		printf("[%x] %d -> %d\n", pthread_self(), n, sum);
		pthread_mutex_unlock(&mutex);
	}
}
