#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int,char*,int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int,int); //클라이언트가 종료했을 때 클라이언트 정보 삭제
pthread_t thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define MAX_CLIENT 10
#define MAX_PLACE 3
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
int num[3]={0,};
struct list {
    int id;
    char name[CHATDATA];
};		
char    nick[CHATDATA];
struct list list_c[MAX_PLACE][MAX_CLIENT];
char    whisper[1024];
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
char	room[]="select room 1~3.\n";
char    roomNum[5];

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
    for(i = 0; i < MAX_PLACE; i++)
		for(j=0;j<MAX_CLIENT;j++)
        	list_c[i][j].id = INVALID_SOCK;

    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
	read(c_socket,nick,sizeof(nick));
	write(c_socket,room,strlen(room));
	read(c_socket,roomNum,sizeof(roomNum));

        res = pushClient(c_socket,nick,atoi(roomNum)-1);
		
	if(res < 0) {
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {		
            write(c_socket, greeting, strlen(greeting));
	    pthread_create(&thread,NULL,do_chat,(void *)&c_socket);
	} 
    }
}

void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n, j;
    int place=atoi(roomNum)-1;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {	
	    if(!strncasecmp(chatData,"/w",strlen("/w"))) {
		char *token;
		char *str[4];
		int i = 0;
		token = strtok(chatData, " ");
		while(token != NULL){
		    str[i++] = token;
		    token = strtok(NULL, " ");
		}			
		for(i=0;i<10;i++) {
		    for(j=0;j<3;j++){
		        if(!strncmp(list_c[j][i].name,str[1],strlen(str[1]))) {
			    sprintf(whisper,"[whisper:%s] : %s",str[3],str[2]);				
			    write(list_c[j][i].id,whisper,strlen(whisper));
		        }
		    }
		}
	    } else {
		for(i=0;i<MAX_CLIENT;i++) {
		    write(list_c[place][i].id,chatData,n);
	    	}
	    }
            if(strstr(chatData, escape) != NULL) {
                popClient(c_socket,place);
                break;
	    }
        }
    }
}
int pushClient(int c_socket,char * name,int roomNum) {
	int result;
	int i;
    
	if(num[roomNum]>=MAX_CLIENT) {
		return -1;
	} else {
		for(i=0;i<MAX_CLIENT;i++) {
		    if(list_c[roomNum][i].id==-1) {
			result=i;
			strcpy(list_c[roomNum][result].name,name);
			break;
		    }
		}
	    pthread_mutex_lock(&mutex);
	    list_c[roomNum][result].id=c_socket;
	    num[roomNum]++;
	    pthread_mutex_unlock(&mutex);
	    return 0;
	}
}

int popClient(int c_socket,int roomNum)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0;i<MAX_CLIENT;i++) {	
	    if(list_c[roomNum][i].id==c_socket) {
		list_c[roomNum][i].id=INVALID_SOCK;
	    }	
	}
	num[roomNum]--;
	pthread_mutex_unlock(&mutex);
	close(c_socket);
}

