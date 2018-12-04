#include <stdio.h>
#include <pthread.h>

void *do_sum(void *val); //1부터 val까지의 합을 계산하는 함수

int main(){
	int status;
	pthread_t thread_id[3];
	int a = 4;
	int b = 5;
	int c = 6;

	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a); //int 타입의 a를 void*로 변환하고 int의 주솟값으로 수정
	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	
	pthread_join(thread_id[0], (void **)&status);
	pthread_join(thread_id[1], (void **)&status);
	pthread_join(thread_id[2], (void **)&status);
	printf("main program end.\n");
	return 0;
}

void *do_sum(void *val){
	int i;
	int sum = 0;
	int max = *((int *)val);
	for(i=0; i < max; i++){
		sum = sum + i;
		printf("%d - add %d\n", max, i);
		sleep(1);
	}
	printf("%d - sum is %d\n", max, sum);
}
