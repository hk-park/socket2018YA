#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //초기값으로 초기화
void *do_sum(void *val);

int main() {
	int status;
	pthread_t thread_id[2];
	int a=5;
	int b=10;

	status=pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	status=pthread_create(&thread_id[1], NULL, do_sum, (void *)&b);

	pthread_join(thread_id[0], (void **)&status);
	pthread_join(thread_id[1], (void **)&status);

	printf("main program END\n");
	pthread_mutex_destroy(&mutex); //선언해놓은걸 해제, 파괴
	return 0;
}

void *do_sum(void *val) {
	int i;
	int n = *((int *)val);
	pthread_mutex_lock(&mutex);
	for(i=0; i<n; i++) { //업데이트
		printf("[%x] sum: %d\n", pthread_self(), sum);
		sum++;
	}
	pthread_mutex_unlock(&mutex); //변수 업데이트 이후 언락
}

