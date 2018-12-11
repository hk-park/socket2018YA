#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_thread(void *);// 스레드가 수행할 함수

//void * : java의 Object와 비스 모든 타입으로 형변환 가능

int main(){
	int status;
	pthread_t thread_id;
	status = pthread_create(&thread_id, NULL, func_thread, NULL);
	if(status != 0){
		printf("[ERROR] thread create fail. status = %d\n",status);
		exit(1);
	}
	printf("thread %d is created.\n",thread_id);
	printf("main funtion end.\n");
	
}

void *func_thread(void *argv){
	int i = 0;

	while(i<10){
		i++;
		printf("thread %d th executing... \n",i);
		sleep(1);
	}

}