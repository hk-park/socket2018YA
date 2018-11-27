#include <stdio.h>
#include <signal.h>

int a = 1;

void sig_handler(int signo);

int main(){
	signal(SIGINT, sig_handler); //첫번째 인자의 시그널을 받았을 때 두번째 인자의 프로그램을 수행하라.SIGINT : ctrl+c가 눌렸을 때 발생하는 신호

    while(1){
		if(a==11){
			printf("당신은 Ctrl+C를 10번 누르셨습니다.\n");
			break;
		}
	}
	return 0;
}

void sig_handler(int signo){
        printf("%dth signal received\n",a);
        a++;
}
