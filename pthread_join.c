#include <stdio.h>
#include <pthread.h>

void *do_sum(void *val); //1부터 val까지의 합을 계산

main(){
	int status;
	pthread_t thread_id[3];
	int a = 4, b = 5, c = 6;
	status = pthread_create(&thread_id[0], NULL, do_sum, (void *)&a);
	status = pthread_create(&thread_id[1], NULL, do_sum, (void *)&b);	
	status = pthread_create(&thread_id[2], NULL, do_sum, (void *)&c);

	pthread_join(thread_id[0], (void **)&status);
	pthread_join(thread_id[1], (void **)&status);
	pthread_join(thread_id[2], (void **)&status);
	printf("mainprogram end.\n");	
	
}

void *do_sum(void *val){
	int i, sum = 0, max = *((int *)val);
	for(i=0;i<max;i++){
		sum += i;
		printf("%d - add %d\n", max, i);
		sleep(1);
	}
	printf("%d - sum is %d\n", max, sum);
}

