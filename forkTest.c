#include <stdio.h>

int main(){
	int pid;
	int a = 10;
	int b = 20;

	a = a+1;
	pid = fork();
	// pid = 자식 프로세스의 프로세스ID(PID) if 부모 프로세스
	// pid = 0 : 자식 프로세스
	// pid = -1 : fork() 실패

	if(pid>0){ // 부모 프로세스
		a=a+1;
		sleep(30);
		printf("PARENT\n");
	}
	else if(pid==0){ // 자식 프로세스
		b=b+1;
		printf("CHILD\n");
	}
	else{ // 실패
		fprintf(stderr, "Failed fork()\n");
	}
	printf("[%d] a = %d / b = %d\n", getpid(), a, b);

	return 0;
}
