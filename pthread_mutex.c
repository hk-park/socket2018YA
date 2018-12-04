#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *do_sum(void *val);

int main(){
	int status;
	pthread_t thread_id[2];
	int a = 5;
	int b = 10;
	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&b);
	
	pthread_join(thread_id[0], (void **)&status);
	pthread_join(thread_id[1], (void **)&status);
	
	printf("main program end\n");
	pthread_mutex_destroy(&mutex);	

	return 0;
}

void *do_sum(void *val){
	int i;
	int n = *((int *)val);
	pthread_mutex_lock(&mutex);

	for(i=0;i<n;i++){
		printf("[%x] sum: %d\n", pthread_self(), sum);
		sum++;
	}
	pthread_mutex_unlock(&mutex);
}
