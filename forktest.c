#include<stdio.h>

int main(){
	int pid;
	int a=10;
	int b=20;//스택에 저장됨. 지역변수/정적변수이므로
	
	a=a+1;
	pid=fork();
	//pid=자식프로세스의 프로세스아이디(pid) if 부모 프로세스
	//pid=0 if 자식프로세스
	//pid=-1 if fork()함수실패
	if(pid>0){//부모프로세스
		a=a+1;
		printf("[PARENT] a=%d. b=%d\n",a,b);
	}else if(pid==0){//자식 프로세스
		b=b+1;
		printf("[CHILD] a=%d, b=%d\n",a,b);
	}else{//fork()실패
		printf("fork() Failed\n");
	}
	return 0;
}
