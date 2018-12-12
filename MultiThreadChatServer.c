#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

void *do_chat(void *);
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제

pthread_t thread;
pthread_mutex_t mutex;

#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000

int list_c[MAX_CLIENT];
char userName[CHATDATA] = " ";
char escape[] = "exit";
char greeting[] = "Welcome to chatting room\n";
char CODE200[] = "Sorry No More Connection\n";
int main(int argc, char *argv[]) {
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int i, j, n;
	int res;
	//
	int status;
	int u_socket;
	//
	//
	
	if(pthread_mutex_init(&mutex, NULL) != 0) {
		printf("Can not create mutex\n");
		return -1;
	}
	
	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
	
	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
	if(listen(s_socket, MAX_CLIENT) == -1) {
		printf("listen Fail\n");
		return -1;
	}

	for(i=0; i<MAX_CLIENT; i++)	list_c[i] = INVALID_SOCK;

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
		if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면
			write(c_socket, CODE200, strlen(CODE200)); //더이상 연결은 불가능하다고 출력
			close(c_socket);
		} else { //클라이언트가 정상적으로 접속했다면
			write(c_socket, greeting, strlen(greeting)); //환영의 문구를 출력
			status = pthread_create(&thread, NULL, do_chat, (void *)&c_socket);

			//pthread_create with do_chat function
		}
	}
}

void *do_chat(void *arg) {
	int c_socket = *((int *)arg);
	char chatData[CHATDATA];

	//
	char *uName, *chat;
	//
	int i, n;
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n=read(c_socket, chatData, sizeof(chatData))) > 0) {
			//
			//write chatData to all clients
			chatData[n] = '\0';
			printf("%s", chatData);
			uName = strtok(chatData, " ");
			chat = strtok(NULL, " ");
			for(i=0; i<MAX_CLIENT; i++) {
				write(userName, chat, sizeof(chat));
				if(chat == NULL) {
					for(i=0; i<MAX_CLIENT; i++) {
						if(userName != INVALID_SOCK)	write(userName, chatData, n);
					}
				}
			}
			//
			//////////////////////////////
			if(strstr(chatData, escape) != NULL) { //사용자가 exit라 입력시
				popClient(c_socket); //Client를 내보냄
				break;
			}
		}
	}
}

int pushClient(int c_socket) {
	//
	int i;
	char uName[CHATDATA];

	memset(uName, 0, sizeof(uName));
	
	for(i=0; i<MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
		if(list_c[i] == INVALID_SOCK) {
			list_c[i] = c_socket;
			if(read(c_socket, uName, sizeof(uName)) > 0)	strcpy(userName, uName);
			printf("%d %s\n", list_c[i], userName);
			pthread_mutex_unlock(&mutex);
			
			return i;
		}
		pthread_mutex_unlock(&mutex);
	}
	
	//ADD c_socket to list_c array.
	//return -1, if list_c is full
	//
	if(i == MAX_CLIENT)	return -1;
	//return the index of list_c which c_socket is added.

}

int popClient(int c_socket) {
	//
	//
	int i;
	close(c_socket);
	for(i=0; i<MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
		if(c_socket == list_c[i]) {
			list_c[i] = INVALID_SOCK;
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}
	//
	return 0;
	//REMOVE c_socket from list_c array
}


