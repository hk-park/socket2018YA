#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *func_thread(void*);//스레드가 수행 할 함수
//void* : Java의 object속성과 같음 (반환 시, 데이터 타입을 형변환하여 반환 할 수 있음)

int main(){
	int status;
	pthread_t thread_id;

	status = pthread_create(&thread_id, NULL, func_thread, NULL);
//int pthread_create(1.스레드 식별 포인터, 2.스레드 속성, 3.스레드 생성함수, 4.함수에 전달할 인자);
//int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start_routin)(void*), void *arg);

	if(status != 0){
		printf("[ERROR!] : thread create fail. status = %d\n",status);
		exit(1);
	}


	printf("Thread %x is created. \n",thread_id);
	pthread_join(thread_id, (void **)&status);
	sleep(2);
	printf("Main function end\n");
}
void *func_thread(void *argv){
	int i = 0;
	while(i<10){
		i++;
		printf("Thread %dth executing..\n",i);
		sleep(1);	
	}
}

