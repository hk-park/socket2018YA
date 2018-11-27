#include <stdio.h>
#include <signal.h>

int a = 1;

void sig_handler(int signo);

main(){
	signal(SIGINT, sig_handler);
	while(1){
		if(a ==11){
			printf("당신은 ctrl+c를 10번 누르셨습니다.\n");
			break;
		}
	}
}

void sig_handler(int signo){
	printf("%dth signal; received\n", a);
	a++;
}
