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
#define BUF_SIZ 10000
#define SIG_KILL_SERVER SIGUSR1
#define SIG_PRINT SIGUSR2
#define CLIENT_NUM 5
#define INVALID_SOCKET -1
#define SAFE_DELETE(t) if(t){free(t); t=NULL;}

//STRUCTs
typedef struct tag_client_info{
    int thread_id;
    int socket;
    char *nickname;
    char *room;
}client_info;

//VARs
client_info g_client_info[CLIENT_NUM];
int g_client_num;

int g_server_socket;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//FUNCs
void *do_service(void *data);
int push_client(client_info *info);
int pop_client(int socket);
void cleanup(client_info *info);

main( )
{
    //s_socket은 전역 영역으로 보냄.
    int   c_socket;
    struct sockaddr_in s_addr, c_addr;
    int   len;

    int pid;
    int tmp=0;
    int i=0;

    client_info c_info;

    pthread_t pthread;
    int thr_status;

    g_server_socket = socket(PF_INET, SOCK_STREAM, 0);

    //init client info
    memset(&c_info,0,sizeof(client_info));
    cleanup(&c_info);
    for(i=0;i<CLIENT_NUM;++i)
    {
	memcpy(&g_client_info[i],&c_info,sizeof(client_info));
    }

    //init socket
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
	len = sizeof(c_addr);

	c_socket = accept(g_server_socket, (struct sockaddr *) &c_addr, &len);
	printf("클라이언트 접속 완료\n");

	thr_status = pthread_create(&pthread,NULL,do_service,&c_socket);
    }
}

int push_client(client_info *info)
{
    int i=0;

    if(g_client_num >= CLIENT_NUM)
    {
	close(info->socket);
	return -1;
    }

    pthread_mutex_lock(&mutex);
    for(i=0;i<CLIENT_NUM;++i)
    {
	if(g_client_info[i].socket== INVALID_SOCKET)
	{
	    memcpy(&g_client_info[i],info,sizeof(client_info));
	    break;
	}
    }
    g_client_num++;
    pthread_mutex_unlock(&mutex);

    return i;
}

int pop_client(int socket)
{
    int i=0;

    if(g_client_num <=0)
	return -1;

    pthread_mutex_lock(&mutex);
    for(i=0;i<CLIENT_NUM;++i)
	if(g_client_info[i].socket == socket)
	{
	    cleanup(&g_client_info[i]);
	    break;
	}
    g_client_num--;
    pthread_mutex_unlock(&mutex);

    close(socket);

    return i;
}

void cleanup(client_info *info)
{
    info->socket=INVALID_SOCKET;
    info->thread_id=INVALID_SOCKET;
    SAFE_DELETE(info->nickname);
    SAFE_DELETE(info->room);
}

void *do_service(void *data)
{
    //client socket
    int c_socket=*((int*)data);

    //for read, write
    char rcvBuffer[BUF_SIZ]="";
    char sendBuffer[BUF_SIZ]="";

    //for string
    int n=0, cntSplits=0;
    char **ppStr=NULL, *strBuild=NULL;

    //for loop
    int i=0;
    int cnt=0;

    //current client index of g_client_info
    int client_idx=0;

    //for login
    client_info c_info;
    char nickname[11]="";
    int strSize=0, flag=0, nickname_bufsize=11;

    //login
    char room_default[]="GLOBAL";
    c_info.socket=c_socket;
    c_info.thread_id=pthread_self();
    c_info.nickname=malloc(1);
    c_info.nickname[0]='\0';

    //receive nickname
    while(1)
    {	
	memset(nickname,'\0',nickname_bufsize);
	if( (n=read(c_socket,nickname,nickname_bufsize-1)) <= 0)
	    exit(-1);

	strSize += n;
	if(nickname[strlen(nickname)-1]=='\n')
	{
	    nickname[strlen(nickname)-1]='\0';
	    strSize--;
	    flag=1;
	}
	c_info.nickname=realloc(c_info.nickname, strSize+1);
	strcat(c_info.nickname,nickname);

	if(flag==1)
	    break;
    }

    printf("nickname : %s\n",c_info.nickname);

    //room
    c_info.room=malloc(strlen(room_default)+1);
    memset(c_info.room,'\0',strlen(room_default)+1);
    strcpy(c_info.room,room_default);

    //push
    push_client(&c_info);
	
    printf("클라이언트 총 갯수 : %d\n",g_client_num);

    for( i=0;i<CLIENT_NUM;++i)
    {
	if(g_client_info[i].socket==c_socket)
	{
	    client_idx=i;
	    break;
	}
    }

    //update
    while(1)
    {
	//init
	memset(rcvBuffer,0,BUF_SIZ);
	memset(sendBuffer,0,BUF_SIZ);
	memset(&n,0,sizeof(int));

	//read	
	n=read(c_socket,rcvBuffer,sizeof(rcvBuffer));
	if(n == 0)
	{
	    printf("received FIN.\n");
	    pop_client(c_socket);
	    return NULL;
	}
	else if(n<0)
	{
	    printf("error code : %d\n",n);
	    pop_client(c_socket);
	    exit(-1);
	}

	rcvBuffer[n]='\0';

	//COMMANDs
	if(soc_msgcmp(rcvBuffer,"/"))
	{
	    if(soc_msgcmp_full(&rcvBuffer[1],"w"))
	    {
		ppStr=soc_strsplit(rcvBuffer," ",&cntSplits);
		strBuild=soc_strBuildFromArray(ppStr,cntSplits,2);

		if(cntSplits <3)
		    continue;

		for(i =0 ;i<CLIENT_NUM;++i)
		{
		    if(g_client_info[i].socket != INVALID_SOCKET)
		    {
			if(!strcmp(ppStr[1],g_client_info[i].nickname))
			{
			    sprintf(sendBuffer,"wisper : %s to %s> %s",g_client_info[client_idx].nickname, ppStr[1], strBuild);
			    soc_write(g_client_info[i].socket,sendBuffer);
			    break;
			}
		    }
		}
		soc_freeCharPtrPtr(&ppStr,cntSplits);
		soc_freeCharPtr(&strBuild);
	    }
	    else if(soc_msgcmp_full(&rcvBuffer[1],"create_room"))
	    {

	    }
	    else if(soc_msgcmp_full(&rcvBuffer[1],"join_room"))
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

			soc_write(g_client_info[i].socket,"kill server commited\n");
			close(g_client_info[i].socket);
			cleanup(&g_client_info[i]);
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
		    printf("nickname: %s\n",g_client_info[i].nickname);
		    printf("room: %s\n",g_client_info[i].room);
		} 
		puts("=================");
	    }
	}
	else
	{

	    sprintf(sendBuffer,"[%s] %s> %s",g_client_info[client_idx].room, g_client_info[client_idx].nickname,rcvBuffer);

	    for(i=0;i<CLIENT_NUM;++i)
	    {
		if(g_client_info[i].socket != INVALID_SOCKET && 
			g_client_info[i].socket != c_socket)
		{
		    soc_write(g_client_info[i].socket,sendBuffer);
		}
	    }
	}
    }

    return NULL;
}

