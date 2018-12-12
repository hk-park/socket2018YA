lude <stdio.h>
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
struct list_c_struct
{
	char nickname[20];
	int connect;
	int step;
	int channel;
}list_c[MAX_CLIENT];
struct channel_struct
{
	char name[64];
	int member;
}channel[MAX_CLIENT];

char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
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
	{
        list_c[i].connect = INVALID_SOCK;
        list_c[i].channel = -1;
		channel[i].member = 0;
	}
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            pthread_create(&thread, NULL, do_chat, (void *) &c_socket);
        }
    }
}
void *do_chat(void *arg)
{
   int c_socket = *((int *)arg);
   char chatData[CHATDATA];
   int i, n;
	char temp_string[CHATDATA];
	char * target;
	char * message;
	int this;
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			for (i = 0; i < MAX_CLIENT; i++){
				if (list_c[i].connect == c_socket) {
					this = i;
					break;
				}
			}
			if(list_c[this].step == 0)
			{
				target = strtok(chatData, "\n");
				strcpy(list_c[this].nickname, target);
				strcpy(temp_string, "");
				for(i = 0; i < MAX_CLIENT; i++)
				{
					if(channel[i].member != 0)
						sprintf(temp_string, "%s[%d]%s\n",temp_string, i, channel[i].name);
				}
				list_c[this].step = 1;
				strcat(temp_string, "create chat room enter: '/create [chat room name]'\njoint the chat room: enter chat 'id'\n");
				write(c_socket, temp_string, strlen(temp_string));
			}
			else if(list_c[this].step == 1)
			{
				if(!strncasecmp(chatData, "/create ", 8))
				{
					strtok(chatData, " ");
					message = strtok(NULL, "\n");
					strcpy(channel[this].name, message);
					channel[this].member = 1;
					list_c[this].channel = this;
					sprintf(temp_string, "[Notice] [%s]'s chat room join\n", channel[this].name);
					write(c_socket, temp_string, strlen(temp_string));
					for (i = 0; i < MAX_CLIENT; i++){
						if (list_c[i].connect != INVALID_SOCK && list_c[i].channel == list_c[this].channel) {
							sprintf(temp_string, "[Notice] [%s]'s Welcome!!\n", list_c[this].nickname);
							write(list_c[i].connect, temp_string, strlen(temp_string));
						}
					}
				}
				else
				{
					message = strtok(chatData, "\n");
					list_c[this].channel = atoi(message);
					channel[list_c[this].channel].member ++;
					for (i = 0; i < MAX_CLIENT; i++){
						if (list_c[i].connect != INVALID_SOCK && list_c[i].channel == list_c[this].channel) {
							sprintf(temp_string, "[Notice] [%s]'s Welcome!!\n", list_c[this].nickname);
							write(list_c[i].connect, temp_string, strlen(temp_string));
						}
					}
				}
				list_c[this].step = 2;
			}
			else if(list_c[this].step == 2)
			{
				if(!strncasecmp(chatData, "/quit", 5))
				{
					for (i = 0; i < MAX_CLIENT; i++){
						if (list_c[i].connect != INVALID_SOCK && list_c[i].channel == list_c[this].channel) {
							sprintf(temp_string, "[Notice] [%s]'s quit!!\n", list_c[this].nickname);
							write(list_c[i].connect, temp_string, strlen(temp_string));
						}
					}
					list_c[this].step = 1;
					channel[list_c[this].channel].member --;
					list_c[this].channel = -1;
					strcpy(temp_string, "");
					for(i = 0; i < MAX_CLIENT; i++)
					{
						if(channel[i].member > 0)
							sprintf(temp_string, "%s[%d]%s\n",temp_string, i, channel[i].name);
					}
					list_c[this].step = 1;
					strcat(temp_string, "create chat room enter: '/create [chat room name]'\njoint the chat room: enter chat 'id'\n");
					write(c_socket, temp_string, strlen(temp_string));
				}
				else if(!strncasecmp(chatData, "/w ", 3))
				{
					strtok(chatData, " ");
					target = strtok(NULL, " ");
					message = strtok(NULL, "\0");
					printf("%s %s", target, message);
					for (i = 0; i < MAX_CLIENT; i++){
						if (list_c[i].connect != INVALID_SOCK) {
							if(!strcmp(list_c[i].nickname, target))
							{
								sprintf(temp_string, "[%s's whisper] %s", list_c[this].nickname, message);
								write(list_c[i].connect, temp_string, strlen(temp_string));
								break;
							}
						}
					}
				}
				else
				{
					for (i = 0; i < MAX_CLIENT; i++){
						if (list_c[i].connect != INVALID_SOCK && list_c[i].channel == list_c[this].channel) {
							sprintf(temp_string, "[%s] %s", list_c[this].nickname, chatData);
							write(list_c[i].connect, temp_string, strlen(temp_string));
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
}
int pushClient(int c_socket) {
    //ADD c_socket to list_c array.
    //
    ///////////////////////////////
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.
	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < MAX_CLIENT; i++)
	{
		if(list_c[i].connect == INVALID_SOCK)
		{
			list_c[i].connect = c_socket;
			list_c[i].step = 0;
			pthread_mutex_unlock(&mutex);
			return i;
		}
	}
	pthread_mutex_unlock(&mutex);
	return -1;
}
int popClient(int c_socket)
{
    close(c_socket);
    //REMOVE c_socket from list_c array.
    //
    ///////////////////////////////////
	int i;
	int this;
	char temp_string[CHATDATA];
	pthread_mutex_lock(&mutex);
	for(i = 0; i < MAX_CLIENT; i++)
	{
		if(list_c[i].connect == c_socket)
		{
			list_c[i].connect = INVALID_SOCK;
			channel[list_c[i].channel].member --;
			list_c[i].channel = -1;
			this = i;
			break;
		}
	}
	pthread_mutex_unlock(&mutex);
	for (i = 0; i < MAX_CLIENT; i++){
		if (list_c[i].connect != INVALID_SOCK && list_c[i].channel == list_c[this].channel) {
			sprintf(temp_string, "[Notice] [%s]'s quit!!\n", list_c[this].nickname);
			write(list_c[i].connect, temp_string, strlen(temp_string));
		}
	}
	return 0;
}
