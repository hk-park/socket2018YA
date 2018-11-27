#include <stdio.h>
#include <signal.h>

int a = 1;

void sig_handler(int signo) 
{
	printf("%dth signal received\n", a);
	a++;
}

int main()
{
	signal(SIGINT, sig_handler); //SIGINT를 받았을 때 sig_handler을 실행
	while(1) {
		if (a == 11) {
			printf("당신은 ctrl+C 를 10번 누르셨습니다.\n");
			break;
		}
	}
	return 0;
}


