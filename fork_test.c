#include <stdio.h>

int main() {
	int pid;	//프로세스 아이디
	int a=10;	//stack에 저장됨
	int b=20;	//stack에 저장됨
	
	a = a+1;	//a=11
	pid = fork();	//pid = 자식 프로세스의 아이디(pid) if 부모 프로세스
			//pid = 0 if 자식 프로세스
			//pid = -1 if fork() 함수 실패
	if(pid > 0) {	//부모 프로세스
		a = a+1;
		sleep(30);
		printf("[PARENT] a = %d, b = %d\n", a, b);
	} else if(pid == 0) {	//자식 프로세스
		b = b+1;
		sleep(20);
		printf("[CHILD] a = %d, b = %d\n", a, b);
	} else {	//실패
		printf("fork() Failed\n");
	}
	return 0;

}
