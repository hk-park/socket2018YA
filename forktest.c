#include <stdio.h>

int main() 
{
	int pid;
	int a = 10;
	int b = 20;

	a = a + 1;
	pid = fork();
	//프로그램 카운터는 여기부터 실행
	//pid = 자식 프로세스의 프로세스 아이디(pid) if 부모 프로세스
	//pid = 0 if 자식 프로세스
	//pid = -1 if fork() 함수 실패
	if(pid > 0) //부모 프로세스
	{
		a = a + 1;
		sleep(20);
		printf("[PARENT] a = %d, b = %d\n", a, b); //a=12,b=20	
	} else if (pid == 0) //자식 프로세스
	{	
		b = b + 1;
		sleep(30);
		printf("[CHILD] a = %d, b = %d\n", a, b); //a=11, b=21
	} else  //fork()실패
	{
		printf("fork() Failed\n");
	}
	
	return 0;
}
