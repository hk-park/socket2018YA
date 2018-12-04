#include<stdio.h>
#include<pthread.h>

void *do_sum(void *val);//1부터 val의 값까지의 합 계산 함수

int main(){
	int status;
	pthread_t thread_id[3];
	int a = 4;
	int b = 5;
	int c = 6;

	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
//void* 형으로 형 변환시, a의 int 값을 보존 하기 위해 & 붙이기
	status = pthread_create(&thread_id[1], NULL, do_sum, (void *)&b);
	status = pthread_create(&thread_id[2], NULL, do_sum, (void *)&c);
	
	pthread_join(thread_id[0], (void **)&status);
        pthread_join(thread_id[1], (void **)&status);
        pthread_join(thread_id[2], (void **)&status);

	printf("Main Program end\n");
	return 0;
}

void *do_sum(void *val){
	int i;
	int sum = 0;
	int max = *((int *)val);

	for(i=0; i<max; i++){
		sum = sum + 1;
		printf("%d - add %d \n",max,i);
		sleep(1);
	}
	printf("MAX :%d SUM :%d \n",max, sum);	
}
