#include <stdio.h>

int main(){
	int pid; //스택에 저장
	int a=10; //스택에 저장
	int b=20; //스택에 저장
	
	a=a+1;
	pid=fork(); //fork()는 2개 실행되도록. 프로그램 카운터는 바로 밑부터 시작
	//pid=자식프로세스의 프로세스아이디(pid)값 if 부모 프로세스
	//pid=0 if 자식 프로세스
	//pid=-1 if fork() 함수 실패
	
	if(pid>0){
		a=a+1;
		sleep(30);
		printf("[PARENT] a = %d, b = %d\n", a, b);
	}//부모프로세스에서는 a=12 b=20. 프로세스 확인 위해 sleep 추가. 자다가 출력하도록
	else if(pid==0){
		b=b+1;
		sleep(20);
		printf("[CHILD] a = %d, b = %d\n", a, b);
	}//자식프로세스에서는 a=11 b=21
	else{
		printf("fork() Failed\n");
	}//fork() 실패

	return 0;
}
