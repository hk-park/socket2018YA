#include<stdio.h>

int main(){
	int pid;
	int a = 10 ;
	int b = 20 ;
	
	a = a+1;
	pid = fork();
	if(pid>0){
		a=a+1;
		sleep(30);
		printf("[PARENT] a = %d , b = %d\n",a,b);	
	}else if(pid==0){
		b=b+1;
		sleep(20);

		printf("[CHILD] a = %d, b = %d\n",a,b);
	}else{ 
		printf("fork() Failed\n");
	}
		printf("[%d] a= %d / b = %d\n",getpid(),a,b);
	return 0;
}
