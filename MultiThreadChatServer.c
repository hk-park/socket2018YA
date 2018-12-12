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
//int    list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
struct list {
	int id;
	char nickname[CHATDATA];
};		
char nick[CHATDATA];
struct list list_c[MAX_PLACE][MAX_CLIENT];
char test[1024];
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
char	three[]="1~3번 방 중 하나를 선택하세요.\n";
char placenum[10];

int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n, g;
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
		g=read(c_socket,nick,sizeof(nick));
		write(c_socket,three,strlen(three));
		g=read(c_socket,placenum,sizeof(placenum));

        res = pushClient(c_socket,nick,atoi(placenum)-1); //접속한 클라이언트를 list_c에 추가
		
		 if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {		
			printf("%d대화방 : 현재 접속중인 인원은 %d명 입니다.\n",atoi(placenum),num[atoi(placenum)-1]);
            write(c_socket, greeting, strlen(greeting));
            //pthread_create with do_chat function.
			pthread_create(&thread,NULL,do_chat,(void *)&c_socket);
		}
        
    }
}

void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
	int place=atoi(placenum)-1;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {	
				if(!strncasecmp(chatData,"/귓속말",strlen("/귓속말"))) {
					char *token;
					char *str[4];
					int i = 0;
					token = strtok(chatData, " ");
					while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
					}			
					for(i=0;i<10;i++) {
						if(!strncmp(list_c[place][i].nickname,str[1],strlen(str[1]))) {
							sprintf(test,"[귓속말] %s님 : %s",str[3],str[2]);
							
							write(list_c[place][i].id,test,strlen(test));
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
int pushClient(int c_socket,char * nickname,int placenum) {
	int result;
	int i;
    //ADD c_socket to list_c array.
	if(num[placenum]>=MAX_CLIENT) {
		return -1;
	} else {
		for(i=0;i<MAX_CLIENT;i++) {
			if(list_c[placenum][i].id==-1) {
				result=i;
				strcpy(list_c[placenum][result].nickname,nickname);
				break;
			}
		}
		pthread_mutex_lock(&mutex);
		list_c[placenum][result].id=c_socket;
		num[placenum]++;
		pthread_mutex_unlock(&mutex);
		return 0;
	}
}

int popClient(int c_socket,int placenum)
{
	int i;
	pthread_mutex_lock(&mutex);
	 for(i=0;i<MAX_CLIENT;i++) {	
		if(list_c[placenum][i].id==c_socket) {
			list_c[placenum][i].id=INVALID_SOCK;
		}	
	}
	num[placenum]--;
	printf("%d대화방 : 현재 접속중인 인원은 %d명 입니다.\n",placenum,num[placenum]);
	pthread_mutex_unlock(&mutex);
	close(c_socket);
}
