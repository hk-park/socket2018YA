#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>


void *func_thread(void *);//스레드가 수행할 함수
//void *:java의 object와 비슷--모든데이터타입으로전달가능 int,char,struct...

int main(){
	int status;
	pthread_t thread_id;
	status=pthread_create(&thread_id,NULL,func_thread,NULL);
	if(status!=0){
		printf("[ERROR] thread create fail status = %d\n",status);
		exit(1);
	}
	printf("thread %s is created\n",thread_id);
	pthread_join(thread_id,(void **)&status);
	printf("main function end\n");
}

void *func_thread(void *argv){
	int i=0;
	while(i<10){
		i++;
		printf("thread %dth executing...\n",i);
		sleep(1);
	}
}
