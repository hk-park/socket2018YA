#include <stdio.h>

<<<<<<< HEAD
int main(){
	int pid;
	int a = 10;
	int b = 20;

	a = a+1;
	pid = fork();
	//pid = 자식 프로세스의 프로세스아이디(pid) if 부모 프로세스
	//pid = 0 if 자식 프로세스
	//pid = -1 if fork() 함수 실패
	if(pid > 0){ // 부모프로세스
		a = a+1;
		sleep(30);
		printf("[PARENT] a = %d, b = %d\n", a, b); 
	}else if(pid == 0){ //자식프로세스
		b = b+1;
		sleep(20);
		printf("[CHILD] a = %d, b = %d\n", a, b);
	}else{ //fork() 실패
=======
int main() {
	int pid;
	int a = 10;
	int b = 20;
	
	a=a+1;
	pid=fork();
	//pid = 자식 프로세스의 pid값 if 부모프로세스
	//pid = 0 if 자식 프로세스
	//pid = -1 if fork() 함수 실패
	if(pid > 0) {
		//부모
		a=a+1;
		sleep(30);
		printf("[PARENT] a=%d, b=%d\n",a,b);
	} else if(pid==0) {
		//자식
		b=b+1;
		sleep(20);
		printf("[CHILD] a=%d, b=%d\n",a,b);
	} else {
		//실패
>>>>>>> fd0e397dc3c28da15ff9db1ea4e5202729833fef
		printf("fork() Failed\n");
	}
	return 0;
}
<<<<<<< HEAD
=======

>>>>>>> fd0e397dc3c28da15ff9db1ea4e5202729833fef
