#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *do_sum(void *val);

int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
	pthread_t thread_id[2];
	int status;
	int val[2] = {5, 10};
	int i;

	for(i=0; i<2; i++){
		status = pthread_create(&thread_id[i], NULL, do_sum, (void*)&val[i]);
		if(status!=0){
			printf("[ERROR] Thread create fail. status=%d\n", status);
			exit(1);
		}
		printf("Thread created.\n");
	}
	for(i=0; i<2; i++)
		pthread_join(thread_id[i], (void**)&status);

	printf("main Line end.\n");
	pthread_mutex_destroy(&mutex);

	return 0;
}

void *do_sum(void *val){
	int i;
	int n = *((int*)val);

	pthread_mutex_lock(&mutex);
	for(i=0; i<n; i++){
		sum++;
		printf("[%x] sum: %d\n", pthread_self(), sum);
		sleep(1);
	}
	pthread_mutex_unlock(&mutex);
	printf("%x is end\n", pthread_self());
}

