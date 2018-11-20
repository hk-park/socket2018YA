#include <stdio.h>

int main()
{
	int pid;
	int a = 10;
	int b = 20;

	a = a + 1;
	pid = fork();
	// fork_test 가 두개가 실행되는 것.
	// 프로그램 카운터가 pid = fork(); 위치부터 
	// fork의 return 값은 :
	// pid = 자식 프로세스의 pid값 if 부모 프로세스
	// pid = 0 if 자식 프로세스
	// pid = -1 if fork() 함수 실패
	
	if (pid > 0)		// 부모 프로세스
	{
		a = a + 1;
		sleep(30);
		printf("[PARENT] a = %d, b = %d \n", a, b);		
	}
	else if (pid == 0)	// 자식 프로세스
	{
		b = b + 1;
		sleep(20);
		printf("[CHILD] a = %d, b = %d \n", a, b);
	}
	else			// fork() 실패
	{
		printf("[ERROR] fork() Failed \n");
	}
	return 0;
}
