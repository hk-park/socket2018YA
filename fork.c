#include<stdio.h>

main()
{
	int pid;
	int a=10;
	int b=20;

	a = a+1;
	pid = fork();
	//pid = 자식 프로세스의 프로세스아이디 if 부모 프로세스
	

	if(pid>0){ //부모프로세스
		a = a+1;
		sleep(3);
	printf("PARENT a= %d, b=%d\n", a, b);
	}else if(pid == 0){// 자식프로세스
	b = b+1;
	sleep(2);
	printf("CHILD a=%d, b=%d\n", a, b);
	} else
		printf("fork() faild");
	return 0;
}

