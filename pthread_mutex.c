#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int sum = 0; //스레드들끼리 공유하는 전역 변수
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex 변수 초기값 설정

void *do_sum(void *val);

int main(){
	int status;
	pthread_t thread_id[2];
	int a = 5;
	int b = 10;
	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	status = pthread_create(&thread_id[1], NULL, do_sum, (void *)&b);
	pthread_join(thread_id[0], (void **)&status);
	pthread_join(thread_id[1], (void **)&status);

	pthread_mutex_destroy(&mutex); //mutex 다 쓴 다음에 파괴파괴!

	printf("main program end.\n");
	return 0;
}

void *do_sum(void *val){
	int i;
	int n = *((int *)val);
	pthread_mutex_lock(&mutex); //mutex 락 걸어줌
	for(i=0; i < n; i++){
		printf("[%x] sum: %d\n", pthread_self(), sum);
		sum++;
	}
	pthread_mutex_unlock(&mutex); //mutex 락 해제
}
