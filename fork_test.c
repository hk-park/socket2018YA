#include <stdio.h>

int main(){
	int pid;
	int a = 10;
	int b = 20;

	a = a+1;
	pid = fork();
	//pid = 자식 프로세스의 프로세스아이디(pid)if 부모 프로세스
	//pid = 0 if
	//pid = -1 fork()

	if(pid > 0){//부모 프로세스
		a = a+1;
		sleep(30);
		printf("[PARENT] a = %d, b = %d\n", a,b);		

	}else if(pid==0){//자식 프로세스
		b = b+1;
		sleep(20);
		printf("[CHILD] a = %d, b = %d\n", a,b);

	}else{ //fork() 실패
		printf("fork() Failed\n");
		
	}
}
