#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void *func_pthread(void*);

int main(){
	int status;
	pthread_t thread_id;

	status = pthread_create(&thread_id, NULL, func_pthread, NULL);

	if(status!=0){
		printf("[ERROR] pthread_create failed\n");
		exit(0);
	}

	printf("thread %x is create\n", thread_id);
	//sleep(5);
	pthread_join(thread_id, (void**)&status);
	printf("main function exit\n");
	return 0;
}

void *func_pthread(void *arg){
	int i=0;

	while(i<10){
		sleep(1);
		i++;
		printf("thread %dth executing...\n", i);
	}
}
