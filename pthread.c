#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * func_pthread(void * arg);//스레드가 수행할 함수
//void * : Java의 Object와 비슷. 모든 타입으로 형변환 가능.
int main() {
	int status;
	pthread_t thread_id;
	
	status=pthread_create(&thread_id,NULL,func_pthread,NULL);
	//성공할 경우 0을 반환. 0이 아닐시 에러메세지 출력.
	if(status != 0) {
		printf("[ERROR] pthread_create Failed status = %d\n",status);
		exit(1);//정상정으로 종료시 0, 에러시 0이 아닌값(1) 
	}

	printf("thread %x is create\n",thread_id);
	//sleep(5);//메인 쓰레드가 종료될 시 자식 쓰레드또한 강제로 종료시키기 때문에 메인쓰레드가 기다려주어야한다.
	pthread_join(thread_id,(void**)&status);
	printf("main function exit\n");
	return 0;
}

void *func_pthread(void *argv) {
	int i=0;
	
	while(i<10) {
		i++;
		printf("thread %dth executing...\n",i);
		sleep(1);//1초에 한번씩 쉬기
	}
}
