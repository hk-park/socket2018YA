#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int ncount = 0;


void *do_sum(void *val);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
	int status;
	pthread_t thread_id[2];

	int a = 5;
	int b = 6;

	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	status = pthread_create(&thread_id[1], NULL, do_sum, (void *)&b);

	pthread_join(thread_id[0],(void **)&status);
        pthread_join(thread_id[1],(void **)&status);

	printf("Main Program End.\n");
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *do_sum(void *val){
	int i;
	int n = *((int *)val);

	for(i=0; i<n; i++){
		pthread_mutex_lock(&mutex);
		printf("[%x]",pthread_self());//스레드 자신의 아이디 값 출력
		printf("SUM : %d\n",ncount);
		sleep(1);
		ncount++;
		pthread_mutex_unlock(&mutex);
	}
}
