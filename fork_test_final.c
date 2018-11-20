#include <stdio.h>



int main()
{
	int pid;
	int a = 10;
	int b = 20;
	//pid와 a,b는 stack에 저장됨
	
	a= a+1;
	pid = fork();
	//pid = 자식 프로세스의 프로세스 아이디(pid) if 부모 프로세스
	////pid = 0 if 자식 프로세스
	//pid = -1 if fork()
	//fork()로 인해서 이때까지의 프로세스가 복제되어 자식 프로세스가 됨.
	//PC는 pic=fork()명령어의 바로 다음 줄에 설정됨.
	//
	if(pid > 0){
		//부모 프로세스
		a = a+1;
		sleep(30);
		printf("[Parent] a = %d, b= %d \n",a,b);
		//a = 11, b=20 출력
	}else if(pid == 0){
		// 자식 프로세스
		b = b +1;
		sleep(20);


		printf("[child] a = %d, b = %d \n", a,b);
	}else{
		//fork()실패
		printf("failed");
	}
	return 0;
}


