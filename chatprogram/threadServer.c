#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include "../lib/utils.h"

//DEFINEs
#define PORT 9000
#define BUF_SIZ 100
#define SIG_KILL_SERVER SIGUSR1
#define SIG_PRINT SIGUSR2
#define CLIENT_NUM 5
#define INVALID_SOCKET -1

//VARs
char rcvBuffer[BUF_SIZ];
char sendBuffer[BUF_SIZ];
char buffer[BUF_SIZ] = "My name is Hongkyu.\n";
char fromStr[BUF_SIZ]="Hi, i'm Client\n";
char toStr[BUF_SIZ]="Hi, i'm Server\n";

typedef struct tag_client_info{
    int thread_id;
    int socket;
}client_info;
client_info g_client_info[CLIENT_NUM];
int g_client_num;

int g_server_socket;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//FUNCs
void *do_service(void *data);
void push_client(client_info *info);
void pop_client(int socket);

//INVALID_SOCKET=-1, push_client, pop_client
//대화방에 들어간 사람들 끼리만 대화하는 기능 구현
//귓속말은 대화방 상관없이 가능.

main( )
{
    //s_socket은 전역 영역으로 보냄.
    int   c_socket;
    struct sockaddr_in s_addr, c_addr;
    int   len;

    int pid;
    int tmp=0;
    int i=0;

    pthread_t pthread;
    int thr_status;

    client_info c_info;

    g_server_socket = socket(PF_INET, SOCK_STREAM, 0);

    //init socket
    memset(g_client_info,INVALID_SOCKET,sizeof(client_info)*5);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    if(bind(g_server_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
	printf("Can not Bind\n");
	return -1;
    }

    if(listen(g_server_socket, CLIENT_NUM) == -1) {
	printf("listen Fail\n");
	return -1;
    }

    while(1) {
	memset(&c_info,INVALID_SOCKET,sizeof(client_info));

	len = sizeof(c_addr);

	c_socket = accept(g_server_socket, (struct sockaddr *) &c_addr, &len);
	printf("클라이언트 접속 완료\n");

	thr_status = pthread_create(&pthread,NULL,do_service,&c_socket);
	c_info.socket=c_socket;
	c_info.thread_id=pthread;
	push_client(&c_info);

	printf("클라이언트 총 갯수 : %d\n",g_client_num);
    }
}

void push_client(client_info *info)
{
    int i=0;

    if(g_client_num >= CLIENT_NUM)
    {
	close(info->socket);
	return;
    }

    pthread_mutex_lock(&mutex);
    for(i=0;i<CLIENT_NUM;++i)
    {
	if(g_client_info[i].socket== INVALID_SOCKET)
	{
	    g_client_info[i].socket=info->socket;
	    g_client_info[i].thread_id=info->thread_id;
	    break;
	}
    }
    g_client_num++;
    pthread_mutex_unlock(&mutex);
}

void pop_client(int socket)
{
    int i=0;

    if(g_client_num <=0)
	return;

    pthread_mutex_lock(&mutex);
    for(i=0;i<CLIENT_NUM;++i)
	if(g_client_info[i].socket == socket)
	{
	    g_client_info[i].socket = INVALID_SOCKET;
	    g_client_info[i].thread_id = INVALID_SOCKET;
	}

    if(g_client_num >0)
	g_client_num--;
    pthread_mutex_unlock(&mutex);

    close(socket);
}

void *do_service(void *data)
{
    //client socket
    int c_socket=*((int*)data);

    //for string length
    int n=0;

    //for loop
    int i=0;
    int cnt=0;

    //update
    while(1)
    {
	//init
	memset(rcvBuffer,0,BUF_SIZ);
	memset(sendBuffer,0,BUF_SIZ);
	memset(&n,0,sizeof(int));

	//read
	if((n=read(c_socket,rcvBuffer,sizeof(rcvBuffer)))<0)
	{
	    printf("can not read");
	    exit(-1);
	}

	rcvBuffer[n]='\0';

	printf("Client Msg : %s",rcvBuffer);

	//COMMANDs
	if(soc_msgcmp(rcvBuffer,"/"))
	{
	    if(soc_msgcmp_full(&rcvBuffer[1],"w"))
	    {
		
	    }
	    else if(soc_msgcmp_full(&rcvBuffer[1],"create_room"))
	    {

	    }
	    else if(soc_msgcmp_full(&rcvBuffer[1],"join"))
	    {
		
	    }
	    else if(soc_msgcmp_full(&rcvBuffer[1],"quit"))
	    { 
		soc_write(c_socket,"quit commited");

		pop_client(c_socket);

		printf("tid [%x] finished\nsocket : %d\n",pthread_self(),c_socket);
		printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n",g_client_num);
		return NULL;
	    }
	    else if(soc_msgcmp_full(&rcvBuffer[1],"kill_server"))
	    {
		cnt=0;
		printf("kill server\n");

		//close all clients
		pthread_mutex_lock(&mutex);
		for(i=0;i<CLIENT_NUM;++i)
		{
		    if(g_client_info[i].socket !=-1 )
		    {
			printf("thread_id : [%x], c_socket : [%d]\n",g_client_info[i].thread_id, g_client_info[i].socket);

			soc_write(g_client_info[i].socket,"kill server commited");
			close(g_client_info[i].socket);

			g_client_info[i].socket = -1;
			g_client_info[i].thread_id = -1;

			cnt++;
		    }
		}
		pthread_mutex_unlock(&mutex);
		pthread_mutex_destroy(&mutex);

		printf("총 클라이언트 개수 : %d\n",g_client_num);
		printf("총 %d 개의 클라이언트 종료됨.\n",cnt);

		//finally close server socket and
		//finish server program.
		close(g_server_socket);
		exit(0);
	    }
	    else if(soc_msgcmp_full(&rcvBuffer[1],"print_socket"))
	    {
		soc_write(c_socket,"printed socket info to server");

		for(i=0;i<CLIENT_NUM;++i)
		{
		    puts("=================");
		    printf("[%d]\n",i);
		    printf("tid: %x\n",g_client_info[i].thread_id);
		    printf("socket: %d\n",g_client_info[i].socket);
		} 
		puts("=================");
	    }
	}
	else
	{
	    //broadcast
	    for(i=0;i<CLIENT_NUM;++i)
	    {
		if(g_client_info[i].socket != INVALID_SOCKET && 
			g_client_info[i].socket != c_socket)
		{
		    soc_write(g_client_info[i].socket,rcvBuffer);
		}
	    }
	}
    }

    return NULL;
}

