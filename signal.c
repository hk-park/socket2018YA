#include <stdio.h>
#include <signal.h>

int a= 1;

void sig_handler(int signo);

int main(){
	signal(SIGINT, sig_handler);
	while(1){
		if(a==11){
			printf("you clicked 'ctrl+c' 10\n");
			break;
		}
	}
}

void sig_handler(int signo){
	printf("%d signal recived\n",a);
	a++;
}
