#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_thread(void *); //스레드가 수행할 함수. 리턴 받는 것도 인자도 void*
//void * : Java의 object라고 생각. 어떤 데이터 타입으로든 형변환 가능. 나중에 뭐가 올지 모를 때 사용

int main(){
	int status;
	pthread_t thread_id;
	status = pthread_create(&thread_id, NULL, func_thread, NULL); //스레드 아이디, 스레드 속성, 스레드 수행
	if(status != 0){ //status 성공적으로 되면 0 리턴
		printf("[ERROR] thread cread fail. status = %d\n", status);
		exit(1);
	}

	printf("thread %x is created\n", thread_id);
	pthread_join(thread_id, (void **)&status); //자식 스레드 종료 기다려줌
	//sleep(5);
	printf("main function end.\n");
}

void *func_thread(void *argv){
	int i = 0;
	while(i < 10){
		i++;
		printf("thread %dth executing...\n", i);
		sleep(1); //1초 쉬겠다는 뜻
	}
}
