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

//VARs
char rcvBuffer[BUF_SIZ];
char sendBuffer[BUF_SIZ];
char buffer[BUF_SIZ] = "My name is Hongkyu.\n";
char fromStr[BUF_SIZ]="Hi, i'm Client\n";
char toStr[BUF_SIZ]="Hi, i'm Server\n";

int g_clientCnt;

typedef struct tag_client_socket{
    int thread_id;
    int socket;
}client_socket_info;
client_socket_info g_client_sockets[CLIENT_NUM];

int g_server_socket;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//FUNCs
void *do_service(void *data);

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

    g_server_socket = socket(PF_INET, SOCK_STREAM, 0);

    //init socket
    memset(g_client_sockets,-1,sizeof(client_socket_info)*5);
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
	printf("클라이언트 접속 완료\n클라이언트 총 갯수: %d\n",++g_clientCnt);

	thr_status = pthread_create(&pthread,NULL,do_service,&c_socket);

	for(i=0;i<CLIENT_NUM;++i)
	{
	    if(g_client_sockets[i].socket== -1)
	    {
		g_client_sockets[i].socket=c_socket;
		g_client_sockets[i].thread_id=pthread;
		break;
	    }
	}

    }
}


void *do_service(void *data)
{
    //client socket
    int c_socket=*((int*)data);

    //get string length
    int n=0;

    //for string split
    char dlim[]=" \n";
    char **ppStr=NULL;
    int cntSplits=0;

    //for FILE
    FILE *fp=NULL;
    char fileBuf[BUF_SIZ]="";
    int fileSize=0;

    //for exec
    char *strBuild=NULL;
    int status=0;

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

	//services
	if(soc_msgcmp(rcvBuffer,"안녕하세요."))
	{
	    soc_write(c_socket,"안녕하세요.만나서 반가워요.");
	}
	else if(soc_msgcmp(rcvBuffer,"이름이 머야?"))
	{
	    soc_write(c_socket,"내 이름은 안재형야.");
	}
	else if(soc_msgcmp(rcvBuffer,"몇 살이야?"))
	{
	    soc_write(c_socket,"나는 25살이야.");
	}
	else if(soc_msgcmp(rcvBuffer,"strlen"))
	{
	    ppStr=soc_strsplit(rcvBuffer,dlim,&cntSplits);
	    if(cntSplits <= 1)
	    {
		soc_write(c_socket,"not enough parameter");
		continue;
	    }

	    sprintf(sendBuffer,"[%s] strlen : %d",ppStr[1],strlen(ppStr[1]));
	    soc_write(c_socket,sendBuffer);
	    soc_freeCharPtrPtr(&ppStr,cntSplits);
	}
	else if(soc_msgcmp(rcvBuffer,"strcmp"))
	{
	    ppStr=soc_strsplit(rcvBuffer,dlim,&cntSplits);
	    if(cntSplits <= 2)
	    {
		soc_write(c_socket,"not enough parameter");
		continue;
	    }

	    sprintf(sendBuffer,"[%s] [%s] strcmp : %d",ppStr[1],ppStr[2],strcmp(ppStr[1],ppStr[2]));
	    soc_write(c_socket,sendBuffer);
	    soc_freeCharPtrPtr(&ppStr,cntSplits);
	}
	else if(soc_msgcmp(rcvBuffer,"readfile"))
	{
	    ppStr=soc_strsplit(rcvBuffer," ",&cntSplits);

	    fp=fopen(ppStr[1],"r");
	    if(!fp)
	    {
		soc_write(c_socket,"file not found");
		continue;
	    }

	    //클라이언트에게 먼저 파일 사이즈 정보를 알려줌
	    fseek(fp,0,SEEK_END);
	    fileSize=ftell(fp);
	    fseek(fp,0,SEEK_SET);
	    sprintf(sendBuffer,"readfile %d",fileSize);
	    soc_write(c_socket,sendBuffer);

	    //클라이언트로부터 준비완료 메세지 수신받고 진행
	    read(c_socket,rcvBuffer,BUF_SIZ);
	    if(soc_msgcmp(rcvBuffer,"ready to go"))
	    {
		while(fgets(sendBuffer,BUF_SIZ,fp))
		{
		    soc_write(c_socket,sendBuffer);
		}
	    }

	    fclose(fp);
	    soc_freeCharPtrPtr(&ppStr,cntSplits);
	}
	else if(soc_msgcmp(rcvBuffer,"exec"))
	{
	    ppStr=soc_strsplit(rcvBuffer," ", &cntSplits);
	    strBuild=soc_strBuildFromArray(ppStr,cntSplits,1);
	    status=system(strBuild);

	    //or 특징 : 첫번째 조건이 만족되면 이후 조건문들 수행 안함
	    if(ppStr == NULL || status != 0 )
	    {
		soc_write(c_socket,"!!exec error occured!!");
		continue;
	    }
	    sprintf(sendBuffer,"[%s] processed",ppStr[1]);
	    soc_write(c_socket,sendBuffer);
	    soc_freeCharPtrPtr(&ppStr,cntSplits);
	    soc_freeCharPtr(&strBuild);
	}
	else if(soc_msgcmp(rcvBuffer,"quit"))
	{ 
	    soc_write(c_socket,"quit commited");

	    printf("tid [%x] finished\nsocket : %d\n",pthread_self(),c_socket);
	    printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n",--g_clientCnt);

	    pthread_mutex_lock(&mutex);
	    for(i=0;i<CLIENT_NUM;++i)
		if(g_client_sockets[i].socket==c_socket)
		{
		    g_client_sockets[i].socket = -1;
		    g_client_sockets[i].thread_id = -1;
		}
	    pthread_mutex_unlock(&mutex);

	    close(c_socket);
	    return NULL;
	}
	else if(soc_msgcmp(rcvBuffer,"kill server"))
	{
	    cnt=0;
	    printf("kill server\n");

	    //close all clients
	    pthread_mutex_lock(&mutex);
	    for(i=0;i<CLIENT_NUM;++i)
	    {
		if(g_client_sockets[i].socket !=-1 )
		{
		    printf("thread_id : [%x], c_socket : [%d]\n",g_client_sockets[i].thread_id, g_client_sockets[i].socket);

		    soc_write(g_client_sockets[i].socket,"kill server commited");
		    close(g_client_sockets[i].socket);

		    g_client_sockets[i].socket = -1;
		    g_client_sockets[i].thread_id = -1;

		    cnt++;
		}
	    }
	    pthread_mutex_unlock(&mutex);
	    pthread_mutex_destroy(&mutex);

	    printf("총 클라이언트 개수 : %d\n",g_clientCnt);
	    printf("총 %d 개의 클라이언트 종료됨.\n",cnt);

	    //finally close server socket and
	    //finish server program.
	    close(g_server_socket);
	    exit(0);
	}
	else if(soc_msgcmp(rcvBuffer,"print socket"))
	{
	    soc_write(c_socket,"printed socket info to server");

	    for(i=0;i<CLIENT_NUM;++i)
	    {
		puts("=================");
		printf("[%d]\n",i);
		printf("tid: %x\n",g_client_sockets[i].thread_id);
		printf("socket: %d\n",g_client_sockets[i].socket);
	    } 
	    puts("=================");
	}
	else
	{
	    soc_write(c_socket,"무슨 말인지 모르겠습니다.");
	}

    }

    return NULL;
}

