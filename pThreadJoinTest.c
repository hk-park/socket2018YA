#include <stdio.h>
#include <pthread.h>

void *do_sum(void *val); // 1부터 var까지의 합 계산

int main(){
	pthread_t thread_id[3];
	int status;
	int val[3] = {4, 5, 6};
	int i;

	for(i=0; i<3; i++){
		status = pthread_create(&thread_id[i], NULL, do_sum, (void*)&val[i]);
		if(status!=0){
			printf("[ERROR] thread create fail.status = %d\n", status);
			exit(1);
		}
		printf("[%x] thread created.\n", thread_id);
	}
	for(i=0; i<3; i++){
		pthread_join(thread_id[i], (void**)&status);
	}
	printf("main program end.\n");
	
	return 0;
}

void *do_sum(void *val){
	int i;
	int sum = 0;
	int max = *((int*)val);
	
	for(i=0; i<max; i++){
		sum += i;
		printf("%d - add %d\n", max, i);
		sleep(1);
	}
	printf("%d - sum is %d\n", max, sum);
}
