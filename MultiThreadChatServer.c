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
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
//int     	list_c[MAX_CLIENT];
struct user{
	char userId[20];
	int	sockNum;
};
struct user user[MAX_CLIENT];
//귓속말을 위한 새로운 배열 선언해주기
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
char temp[20];
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
    for(i = 0; i < MAX_CLIENT; i++)
        user[i].sockNum = INVALID_SOCK;
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		 read(c_socket, temp, sizeof(temp)); //닉네임 추
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            //pthread_create with do_chat function.
			 pthread_create(&thread, NULL, do_chat, (void*)&c_socket);
			 printf("New Client is Coming! c_socket:%d nickname:%s\n",c_socket,temp);
			 memset(temp,0,sizeof(temp));
        }
    }
}
void *do_chat(void *arg)
{
	int c_socket = *((int *)arg);
    char    chatData[CHATDATA];
	int	i, n;
	char	*recv_nick,*send_nick,*message;
	while(1) {
		memset(chatData, 0, sizeof(chatData));
          	if ((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {
				printf("%s\n",chatData);
				if(strstr(chatData,"/w")!=NULL){		   //문장에"/w"귓속말이 있는지 검사
					send_nick=strtok(chatData," "); //보내는사람
					recv_nick=strtok(NULL," ");
					recv_nick=strtok(NULL," ");//받는사람 저장 
					message=strtok(NULL,"\n");
					for (i = 0; i < MAX_CLIENT; i++) {
                   		if (user[i].sockNum != INVALID_SOCK) {
							if(!strcmp(user[i].userId,recv_nick)){
								sprintf(chatData, "%s %s\n\0",send_nick,message);
                    			write(user[i].sockNum, chatData, strlen(chatData));
							}
						}
					}
					if(strstr(chatData, escape) != NULL) {
                		popClient(c_socket);
					break;
					}
				}
			else{
                for (i = 0; i < MAX_CLIENT; i++) {
                   	if (user[i].sockNum != INVALID_SOCK) {
                    	write(user[i].sockNum, chatData, n);
					}
				}

                if(strstr(chatData, escape) != NULL) {
                	popClient(c_socket);
				break;
			}
                        }
            	}
        }
}
int pushClient(int c_socket) {
	int	i;

	for(i = 0; i < MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
		if(user[i].sockNum == INVALID_SOCK) {
			user[i].sockNum = c_socket;
			strcpy(user[i].userId,temp);
			pthread_mutex_unlock(&mutex);
			return i;
		}	
		pthread_mutex_unlock(&mutex);
	}

	if(i == MAX_CLIENT)
		return -1;
}
int popClient(int c_socket)
{
	int i;

	close(c_socket);

	for (i = 0; i < MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
			if ( c_socket == user[i].sockNum ) {
				user[i].sockNum = INVALID_SOCK;
				pthread_mutex_unlock(&mutex);
				break;
			}
		pthread_mutex_unlock(&mutex);
        }
	return 0;
}


