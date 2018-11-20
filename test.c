#include<stdio.h>

int main(){
	int pid;
	int a = 10;
	int b = 20;

	a = a+1;
	pid = fork();
	
	
	if(pid>0)
	{
		a = a+1;
		printf("[PARNET] a= %d, b= %d\n",a,b);
		sleep(30);
	}
	else if(pid = 0)
	{
		b = b+1;
		printf("[CHILD] a = %d, b= %d\n",a,b);
		sleep(20);
	}
	else
	{
		printf("fock is fail\n");
	}
	return 0;
}
