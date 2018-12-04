#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *func_thread(void *);

int main() {
	int status;
	pthread_t thread_id;
	status = pthread_create(&thread_id, NULL, func_thread, NULL);
	if(status != 0) {
		printf("[ERROR] thread create fail. status = %d\n", status);
		exit(1);
	}
	printf("thread %x is created.\n", thread_id);
	pthread_join(thread_id, (void **)&status);
	//sleep(5);
	printf("main function END.\n");
}
void *func_thread(void *argv) {
	int i=0;
	while(i<10) {
		i++;
		printf("thread %dth executing...\n", i);
		sleep(1);
	}
}

