#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * func_thread(void *); // 스레드가 수행할 함수
//void * : Java의 Object와 비슷, 모든 타입으로 형변환 가능

int main()
{
	int status;
	pthread_t thread_id;
	status = pthread_create(&thread_id, NULL, func_thread, NULL); //스레드 생성
	if(status != 0) // 비정상 종료 = 0 아닌 수, 정상종료 = 0
	{
		printf("[ERORR] thread create fail. status %d\n", status);
		exit(1);
	}
	printf("thread %x is created. \n", thread_id);
	pthread_join(thread_id, (void **) &status);
	printf("main function end.\n");
}

void *func_thread(void *argv)
{
	int i=0; 
	while(i < 10) //for문 가능
	{
		i++;
		printf("thread %dth executing... \n", i);
		sleep(1); //1초씩 쉬기, 1초마다 메세지 출력
	}
}
