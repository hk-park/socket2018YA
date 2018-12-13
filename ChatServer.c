#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>

void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제
pthread_t thread;
pthread_mutex_t mutex;
#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000

typedef struct clientlist{
	int c_socket;
	char nickname[20];
	char roomname[20];
}clientlist;
clientlist    client[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
int 	numofcli = 0;
char 	nickname[20];
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";

int main()
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int i, j, n;
	int res;



	if(pthread_mutex_init(&mutex, NULL) != 0 ){
		printf("can not create mutex\n");
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

	for(i = 0; i<MAX_CLIENT; i++){
		client[i].c_socket = INVALID_SOCK;
		strcpy(client[i].roomname, "default");
	}
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
		if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,		printf(" [client]# ", n, rcvBuffer);
		    write(c_socket, CODE200, strlen(CODE200));
		    close(c_socket);
		} else {
			read(c_socket, nickname, sizeof(nickname));
			strcpy(client[numofcli].nickname, nickname);
			printf("새 클라이언트가 접속함[%s]: %d개 접속중\n", client[numofcli].nickname, numofcli+1);
			numofcli++;
		    write(c_socket, greeting, strlen(greeting));
			    //pthread_create with do_chat function.
			pthread_create(&thread, NULL, do_chat, (void*)&c_socket);
			//pthread_join(thread, NULL);
		}
	    }
	close(s_socket);
}

void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
	char checkData[CHATDATA];
	char command[20];
    int i, n;
	int mynumber;
	char myroom[20];
	char *token;
	char *str[3];
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {

			printf("유저입력: %s", chatData);

			chatData[n] = '\0';
			strcpy(command, chatData);
			token = strtok(command, " ");
			token = strtok(NULL, "\0");
			strcpy(command, token);
			for(i=0; i<MAX_CLIENT; i++){
					if(client[i].c_socket==c_socket){
						mynumber = i;
						strcpy(myroom, client[mynumber].roomname);
						break;
					}						
				}
			if(strncasecmp(command, "/w", strlen("/w")) == 0){
				token = strtok(command, " ");
				str[1] = strtok(NULL, " ");
				str[2] = strtok(NULL, "\0");
				for(i=0; i<MAX_CLIENT; i++){
					if(!strcmp(str[1], client[i].nickname)){
						sprintf(chatData, "[%s의 귓속말] %s", str[1], str[2]);
						write(client[i].c_socket, chatData, strlen(chatData));
						break;
					}
					if(i==MAX_CLIENT-1){
						sprintf(chatData, "[!server!] %s 는 없는 사람입니다!\n", str[1]);
						write(c_socket, chatData, strlen(chatData));
					}
				} 
			} else if(strncasecmp(command, "/move", strlen("/move")) == 0){				
				token = strtok(command, " ");
				str[1] = strtok(NULL, "\n");
              sprintf(chatData, "[!server!] 채팅방 %s 에서 %s 로 이동\n", client[mynumber].roomname, str[1]);
				write(c_socket, chatData, strlen(chatData));
				strcpy(client[mynumber].roomname, str[1]);
			}
			else{
				for(i = 0; i<numofcli; i++){
					if( !strcmp(client[i].roomname, myroom))
		    		write(client[i].c_socket, chatData, strlen(chatData));
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
	if(numofcli == MAX_CLIENT)
		return -1;
	else{
		client[numofcli].c_socket = c_socket;
		return c_socket;
	}
    //ADD c_socket to list_c array.
    //
    ///////////////////////////////
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.
}
int popClient(int c_socket)
{
	int i, j;
	for(i=0; i<numofcli; i++){
		if(client[numofcli].c_socket==c_socket){
			{
				client[numofcli].c_socket = -1;
			    close(c_socket);
				
				while(client[i+1].c_socket!=-1){
					client[i].c_socket = client[i+1].c_socket;
					strcpy(client[i].nickname, client[i].nickname);
					i++;
				}
			}
		}
	}
	printf("클라이언트 접속 종료: %d개 접속중\n", --numofcli);
}
