#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_thread(void *); //스레드가 수행할 함수
//void * : Java의 object와 비슷. 모든 타입으로 형변환 가능

int main() {
	int status;
	pthread_t thread_id;
	status = pthread_create(&thread_id, NULL, func_thread, NULL);
	if(status !=0){//대부분의 프로그램이 종료되면 0이 반환된다
		printf("[ERROR] pthread_create failed status = %d\n",status);
		exit(1);
	}

	printf("thread %x is created\n", thread_id);
	pthread_join(thread_id, (void **)&status);
	//sleep(5);
	printf("main function exit\n");

}

void *func_thread(void *argv){
	int i=0;

	while(i<10){
		i++;
		printf("thread %dth executing...\n",i);
		sleep(1);
	}
}
