#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_thread(void *); //스레드가 수행할 함수
//void * : Java의 Object와 비슷 (모든 타입으로 형변환 가능)

int main(){
		int status;
		pthread_t thread_id;
		status = pthread_create(&thread_id, NULL, func_thread, NULL);
	
		if(status != 0){
			printf("[ERROR] thread create fail. status = %d\n", status);
			exit(1); // 프로그램은 정상적으로 종료되면 0을 리턴함. 1은 정상적이지 않음을  표현.
		}

		printf("thread %x is created. \n", thread_id);
		pthread_join(thread_id, (void **)&status); // 자식스레드가 종료될 때가지 메인 스레드 살아있음		
		printf("main funcion end.\n");
		
}

void *func_thread(void *argv){
		int i = 0;
		
		while(i<10){
			i++;
			printf("thread %dth executing...\n", i);
			sleep(1);
		}
}
		
