#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
void *do_chat(void *); 
int pushClient(int); 
int popClient(int);
pthread_t thread;
pthread_mutex_t mutex;
#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
#define USERNAME 100
#define MAX_CHANNEL 5

struct user{
	char name[USERNAME];
	int sock;
	int ch;
};

struct user list_c[MAX_CLIENT];

int chList[MAX_CHANNEL];
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
        list_c[i].sock = INVALID_SOCK;


    for(i = 0; i < MAX_CHANNEL; i++)
	chList[i] = i+1;
	
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); 
        if(res < 0) { 
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            pthread_create(&thread, NULL, do_chat, (void*)&c_socket);
        }
    }
}

void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA], sendData[CHATDATA], tempData[CHATDATA];
    char userChk[USERNAME];
    int i, n, ch;
    char *chk, *uname, *msg;

    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
	    strcpy(tempData, chatData);
	    chk = strtok(tempData, " ");
	    chk = strtok(NULL, " ");
	    if(strncmp(chk, "/w", 2)==0){	
		uname = strtok(NULL, " ");
		msg = strtok(NULL, "\0");

		for(i=0; i<MAX_CLIENT; i++){
			if(list_c[i].sock == c_socket){
				strcpy(userChk, list_c[i].name);
				break;
			}
		}

		for(i=0; i<MAX_CLIENT; i++){
			if(strncmp(list_c[i].name, uname, strlen(uname))==0){
				sprintf(sendData, "[%s] %s", userChk, msg);
				write(list_c[i].sock, sendData, n);
			}
		}
            }else if(strstr(chatData, escape) != NULL) {
                popClient(c_socket);
                break;
            }else{

		for(i=0; i<MAX_CLIENT; i++){
			if(list_c[i].sock==c_socket){
			    ch = list_c[i].ch;
			    break;
			}
		}

    	for(i=0; i<MAX_CLIENT; i++){
			if(list_c[i].sock==INVALID_SOCK)	break;
			if(list_c[i].ch==ch)	write(list_c[i].sock, chatData, n);
	    	}
	    }
        }
    }
}


int pushClient(int c_socket) {
	int i, j, n, ch;
	char nickname[USERNAME];

	n = read(c_socket, nickname, sizeof(nickname));
	nickname[n] = '\0';
	n = read(c_socket, &ch, sizeof(int));

    pthread_mutex_lock(&mutex);
	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock==INVALID_SOCK){
			list_c[i].sock = c_socket;
			strcpy(list_c[i].name, nickname);

			for(j=0; j<MAX_CHANNEL; j++){
				if(ch==chList[j]){
				    list_c[i].ch = ch;
				    break;
				}
			}

			if(j==MAX_CHANNEL)	return -1;

			printf("%s님이 %d번 방에 입장하였습니다.\n", list_c[i].name, list_c[i].ch);
			return i;
		}
	}
	pthread_mutex_unlock(&mutex);
	if(i==MAX_CLIENT)	return -1;
}

int popClient(int c_socket)
{
	int i, j;	

    pthread_mutex_lock(&mutex);
	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock==c_socket){
			printf("%s님이 퇴장하셨습니다.\n", list_c[i].name);
			for(j=i; j<MAX_CLIENT; j++){
				list_c[j].sock=list_c[j+1].sock;
				if(list_c[j].sock==INVALID_SOCK)	break;
			}
		}
	}
	pthread_mutex_unlock(&mutex);
    close(c_socket);
	return j;
}
