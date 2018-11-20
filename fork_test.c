#include <stdio.h>

main(){
	int pid;
	int a = 10;
	int b = 10;

	a = a+1;

	pid=fork();
	if(pid>0){
		a=a+1;
		printf("Parent: ");
	} else if (pid == 00){
		b++;
		printf("Child : ");
	} else {
		printf("failed");
	}
	printf("[%d] a: %d\tb: %d\n", getpid(), a, b);
	sleep(30);
}
