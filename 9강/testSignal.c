#include <stdio.h>
#include <signal.h>

int g_cnt=1;
void sig_handler(int signum)
{
    printf("%dth signal received\n",g_cnt++);
    printf("signal number : %d\n",signum);
}

void main()
{
    signal(SIGINT,sig_handler);
    while(1)
    {
	puts("WHILE START");
	sleep(2);
	puts("WHILE END");
    }
}
    
