#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>

#define CHATDATA 1024
#define IPADDR "127.0.0.1"
#define PORT 9000

void *doSendChat(void *);
void *doRcvChat(void *);

pthread_t thread_1, thread_2;
char escape[] = "/exit";
char nickname[20];

int main(int argc, char *argv[]){
	int c_socket;
	struct sockaddr_in c_addr;
	int len;
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int nfds;
	fd_set read_fds;
	int n;

	c_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	printf("Input Nickname : ");
	scanf("%s", nickname);

	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
		printf("Can not connect\n");
		return -1;
	}
	write(c_socket, nickname, strlen(nickname));
	n = pthread_create(&thread_1, NULL, doSendChat, (void *)&c_socket);
	n = pthread_create(&thread_2, NULL, doRcvChat, (void *)&c_socket);
	pthread_join(thread_1, (void*)&n);

	close(c_socket);
}

void * doSendChat(void *arg){
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int n;
	int c_socket = *((int *) arg);
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n = read(0, chatData, sizeof(chatData))) > 0 ) {
			write(c_socket, chatData, strlen(chatData));
			if(!strncmp(chatData, escape, strlen(escape))) {
				pthread_kill(thread_2, SIGINT);
				break;
			}
		}
	}
}
void *doRcvChat(void *arg){
	char chatData[CHATDATA];
	int n;
	int c_socket = *((int *)arg);
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {
			write(1, chatData, n);
		}
	}
}
