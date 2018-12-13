#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제
pthread_t thread;
pthread_mutex_t mutex;
#define MAX_CLIENT 10
#define NAMELEN 20
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000

typedef struct user_info{
	int socket;
	int room_num;
	char nickname[NAMELEN]; //접속한 클라이언트의 닉네임
}client_info;
char escape[ ] = "exit";
char greeting[ ] = "Welcome to chatting room\n";
char CODE200[ ] = "Sorry No More Connection\n";
client_info client[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열

int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;
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
    pthread_mutex_lock(&mutex);
    for(i = 0; i < MAX_CLIENT; i++)
        client[i].socket = INVALID_SOCK;
	pthread_mutex_unlock(&mutex);
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            //pthread_create with do_chat function.
			 pthread_create(&thread, NULL, do_chat, (void *)&c_socket);
        }
    }
}
void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
	char whisper[CHATDATA];
    int i, n, room_num;
	char *token;
	char *str[3];
	
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
            //write chatData to all clients
			chatData[n] = '\0';
			strcpy(whisper, chatData);
			token = strtok(whisper, " ");
			token = strtok(NULL, "\0");
			strcpy(whisper, token);
			if(strncasecmp(whisper, "/w", strlen("/w")) == 0){
				token = strtok(whisper, " "); // token = "/w"
				str[1] = strtok(NULL, " "); //str[1] = "귓속말 대상"
				str[2] = strtok(NULL, "\0"); //str[2] = "귓속말 내용"
				for(i=0; i<MAX_CLIENT; i++){
					pthread_mutex_lock(&mutex);
					if(!strcmp(str[1], client[i].nickname)){
						sprintf(chatData, "[%s] %s", str[1], str[2]);
						write(client[i].socket, chatData, strlen(chatData));
						pthread_mutex_unlock(&mutex);
						break;
					}
					pthread_mutex_unlock(&mutex);
				} 
			}
			else{
				for(i=0; i<MAX_CLIENT; i++){
					if(client[i].socket == c_socket){
						room_num = client[i].room_num;
						break;
					}
				}
			}
			for(i=0; i<MAX_CLIENT; i++){
				if(client[i].socket != INVALID_SOCK){
					if(client[i].room_num == room_num)
						write(client[i].socket, chatData, strlen(chatData));
				}
			}
			if(strstr(chatData, escape) != NULL) {
				popClient(c_socket);
				break;
           		 }
        	}
    	}
}
int pushClient(int c_socket) {
	int i=0;
	
	for(i = 0; i < MAX_CLIENT; i++){
		pthread_mutex_lock(&mutex);
		if(client[i].socket == INVALID_SOCK){
			client[i].socket = c_socket;
			read(c_socket, client[i].nickname, sizeof(client[i].nickname));
			read(c_socket, &client[i].room_num, sizeof(int));
			pthread_mutex_unlock(&mutex);
			return i;
		}
		pthread_mutex_unlock(&mutex);
	}
	
	if(i == MAX_CLIENT)
		return -1;
    //ADD c_socket to list_c array.
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.
}
int popClient(int c_socket)
{
	int i=0;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < MAX_CLIENT; i++){
		if(c_socket == client[i].socket){
			client[i].socket = INVALID_SOCK;
			break;
		}
	}
	pthread_mutex_unlock(&mutex);
	close(c_socket);
	return 0;
    //REMOVE c_socket from list_c array.	
}
