#include <stdio.h>
#include <signal.h>

int a = 1;

void sig_handler(int signo){ //ctrl+c를 누를 때 마다 동작
	printf("%dth signal received\n", a);
	a++;
}

int main(){
	signal(SIGINT, sig_handler);
	while(10){ //ctrl+c를 10번 누르면 종료
		if(a==11){
			printf("당신은  ctrl+c를 10번 누르셨습니다\n");
			break;
		}
	}
	return 0;
}
