#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_thread(void *); // 스레드가 수행할 함수
// void * : Java Object 개념, 어떤 자료형으로도 변환이 가능

int main(){
	int status;
	pthread_t thread_id;
	status = pthread_create(&thread_id,NULL, func_thread, NULL);
	
	if(status != 0){
		printf("[ERROR] thread create fail. status = %d\n", status);
		exit(1);
	}
	printf("thread %x is created.\n", thread_id);
	for(int i=0; i<10; i++){
		printf("main %d\n", i);
		sleep(1);
	}
	pthread_join(thread_id, (void **)&status);
	//sleep(5);	
	printf("main function endLine.\n");

	return 0;
}

void *func_thread(void *arg){
	int i;
	for(i=0; i<10; i++){
		printf("thread %dth executing..\n", i);
		sleep(1);
	}
}
