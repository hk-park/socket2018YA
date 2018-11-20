#include<stdio.h>

int main(){
	int pid;
	int a = 10;
	int b = 20;

	a = a+1;
	pid = fork();
	//프로그램을 하나 더 생성
	//pid = 자식 프로세스의 프로세스의 아이디(pid) if 부모 프로세스
	//pid = 0 if 자식 프로세스
	//pid = -1 if fork() 함수 실패
	
	if(pid > 0){//부모 프로세스
		a = a+1;
		printf("[PARENT] a = %d, b = %d\n",a,b);
		sleep(30);
	}else if(pid == 0){//자식 프로세스
		b = b+1;
		printf("[CHILD] a = %d. b = %d\n",a,b);
		sleep(20);
	}else{ //실패
		printf("fork() failed\n");
	}
	return 0;
}
