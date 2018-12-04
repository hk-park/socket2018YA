#include <stdio.h>
#include <pthread.h>

void *do_sum(void *val); //1부터 val까지의 합을 계산하는 함수

int main(){
	int thr_id;
	pthread_t p_thread[3];
	int status;
	int a = 4;
	int b = 5;
	int c = 6;
	thr_id = pthread_create(&p_thread[0], NULL, do_sum, (void *)&a);
	thr_id = pthread_create(&p_thread[1], NULL, do_sum, (void *)&b);
	thr_id = pthread_create(&p_thread[2], NULL, do_sum, (void *)&c);
	pthread_join(p_thread[0], (void **)&status);
	pthread_join(p_thread[1], (void **)&status);
	pthread_join(p_thread[2], (void **)&status);
	printf("main program is ended\n");
	return 0;
}

void *do_sum(void *val){
	int i;
	int sum = 0;
	int max = *((int *)val);
	for(i=0;i < max; i++){
		sum = sum + i;
		printf("%d - Add %d\n", max, i);
		sleep(1);
	}
	printf("%d - sum is %d\n", max, sum);
}
