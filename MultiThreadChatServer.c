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

int    list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n"; //클라이언트한테 보내주는 메세지
char    CODE200[ ] = "Sorry No More Connection\n"; //10개 이상의 클라이언트 접속 시 불가함 알려주기
int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;

    if(pthread_mutex_init(&mutex, NULL) != 0) 
	{
        printf("Can not create mutex\n");
        return -1;
	}

    s_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) 
	{
        printf("Can not Bind\n");
        return -1;
	}

    if(listen(s_socket, MAX_CLIENT) == -1)
	{
        printf("listen Fail\n");
        return -1;
	}

    for(i = 0; i < MAX_CLIENT; i++)
        list_c[i] = INVALID_SOCK;

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
        }
    }
}
void *do_chat(void *arg) 
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) { //read로 클라이언트가 보낸 것을 읽고
            //모든 클라이언트에게 메세지 보내기 (write)
            //
            ///////////////////////////////
            if(strstr(chatData, escape) != NULL) {
                popClient(c_socket);
                break;
            }
        }
    }
}

int pushClient(int c_socket) { //현재 접속한 클라이언트를 list_c에 추가
    //ADD c_socket to list_c array.
    //
    ///////////////////////////////
    //-1 리턴해주기, if list_c is full.
    //return the index of list_c which c_socket is added.
}

int popClient(int c_socket) //INVALID_SOCK으로 초기화
{
    close(c_socket);
    //REMOVE c_socket from list_c array.
    //
    ///////////////////////////////////
}
