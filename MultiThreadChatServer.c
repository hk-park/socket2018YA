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
#define PORT 9006


struct user {
	int list_c;	//이전의 list_c 배열과 동
	char list_clientName[CHATDATA];
};
struct user userList[MAX_CLIENT];
// int list_c[MAX_CLIENT];
// char userName[CHATDATA] = " ";

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

	for(i=0; i<MAX_CLIENT; i++)	userList[i].list_c = INVALID_SOCK;

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
		if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면
			write(c_socket, CODE200, strlen(CODE200)); //더이상 연결은 불가능하다고 출력
			close(c_socket);
		} else { //클라이언트가 정상적으로 접속했다면
			write(c_socket, greeting, strlen(greeting)); //환영의 문구를 출력
			//pthread_create with do_chat function
			status = pthread_create(&thread, NULL, do_chat, (void *)&c_socket);

			if(status) {
				printf("[ERROR] thread create fail. status = %d\n", status);
				exit(1);
			}
			//
			//
			//
		}
	}
}

void *do_chat(void *arg) {
	int c_socket = *((int *)arg);
	char chatData[CHATDATA];
	char tempData[CHATDATA];
	//

	char *temp, *send, *whisper, *receive;
	//
	int i, n;
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n=read(c_socket, chatData, sizeof(chatData))) > 0) {
			//write chatData to all clients
			chatData[n] = '\0';
			//printf("%s", chatData);
			strcpy(tempData, chatData);
			temp = strstr(tempData, "/w");
			// temp = strstr(tempData, "/ㅈ");
			if(temp == NULL) {
				for(i=0; i<MAX_CLIENT; i++) {
					if(userList[i].list_c != INVALID_SOCK && userList[i].list_c != c_socket)
						write(userList[i].list_c, chatData, n);
				}
					if(strstr(chatData, escape) != NULL) { //사용자가 exit라 입력시
						popClient(c_socket); //Client를 내보냄
						break;
					}
			} else {
				receive = strtok(chatData, " ");
				whisper = strtok(NULL, "/w ");
			//	whisper = strtok(NULL, "/ㅈ ");
				send = strtok(NULL, "\n");
				for(i=0; i<MAX_CLIENT; i++) {
					if(strcmp(userList[i].list_clientName, whisper) == 0) {
						write(userList[i].list_c, chatData, strlen(chatData));
						write(userList[i].list_c, send, strlen(send));
					}
				}
			}
		}
	}
}

int pushClient(int c_socket) {
	//
	int i;
	char push[CHATDATA];

	memset(push, 0, sizeof(push));
	
	for(i=0; i<MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
		if(userList[i].list_c == INVALID_SOCK) {
			userList[i].list_c = c_socket;
			if(read(c_socket, push, sizeof(push)) > 0)	strcpy(userList[i].list_clientName, push);
			// printf("%d %s\n", list_c[i], userName);
			pthread_mutex_unlock(&mutex);
			return i;
		}
		//printf("사용자가 입장하였습니다.\n");
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
		if(c_socket == userList[i].list_c) {
			userList[i].list_c = INVALID_SOCK;
			//printf("사용자가 퇴장하였습니다.\n");
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}
	//
	return 0;
	//REMOVE c_socket from list_c array
}


