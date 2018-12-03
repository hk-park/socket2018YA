#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <signal.h>
#include <sys/wait.h>
#include "../lib/utils.h"

//DEFINEs
#define PORT 9000
#define BUF_SIZ 100
#define SIG_KILL_SERVER SIGUSR1
#define SIG_PRINT SIGUSR2
#define CLIENT_NUM 5

//GLOBALs
char rcvBuffer[BUF_SIZ];
char sendBuffer[BUF_SIZ];
char buffer[BUF_SIZ] = "My name is Hongkyu.\n";
char fromStr[BUF_SIZ]="Hi, i'm Client\n";
char toStr[BUF_SIZ]="Hi, i'm Server\n";

int g_clientCnt;
int g_fd[2];
int g_ppid=0;

typedef struct tag_client_socket_info{
    int socket;
    int pid;
}client_socket_info;
client_socket_info g_client_sockets[5];

int s_socket;


void do_service(int c_socket);
void sig_child(int signum);
void sig_kill_server(int signum);
void sig_print(int signum);

main( )
{
    //s_socket은 전역 영역으로 보냄.
    int   c_socket;
    struct sockaddr_in s_addr, c_addr;
    int   len;

    int pid;
    int tmp=0;
    int i=0;
    
    signal(SIGCHLD,sig_child);
    signal(SIG_KILL_SERVER, sig_kill_server);
    signal(SIG_PRINT,sig_print);

    pipe(g_fd);

    s_socket = socket(PF_INET, SOCK_STREAM, 0);

    //init socket
    memset(g_client_sockets,-1,sizeof(client_socket_info)*5);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
	printf("Can not Bind\n");
	return -1;
    }

    if(listen(s_socket, CLIENT_NUM) == -1) {
	printf("listen Fail\n");
	return -1;
    }

    while(1) {
	len = sizeof(c_addr);
	c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);

	printf("client connected\n");
	pid=fork();

	if(pid>0)
	{
	    //add client socket to array.
	    for(i=0;i<CLIENT_NUM;++i)
	    {
		if(g_client_sockets[i].socket == -1 
			&& g_client_sockets[i].pid == -1)
		{
		    g_client_sockets[i].socket=c_socket;
		    g_client_sockets[i].pid=pid;
		    break;
		}
	    }

	    printf("현재 %d개의 클라이언트가 접속하였습니다.\n",++g_clientCnt);
	    
	    //don't need this client socket for parent process.
	    close(c_socket);

	    //send ppid to child.
	    g_ppid=getpid();
	    write(g_fd[1],&g_ppid,sizeof(int));
	    continue;
	}
	else if(pid==0)
	{
	    //read ppid from pipe before proceeds service.
	    read(g_fd[0],&g_ppid,sizeof(int));
	    
	    //don't need this server socket copyed from parent process.
	    close(s_socket);

	    //do service.
	    //while loop
	    do_service(c_socket);

	    //quit.
	    exit(0);
	}
    }
    close(s_socket);
}

void sig_child(int signum)
{
    int pid;
    int status;
    int i=0;
    pid=wait(&status); 

    //pid가 -1이면 system 함수에 의해서 생성된 자식 프로세스가 종료되었다는 의미이다.
    //우리가 원하는 것은 system 함수에 의해서 발생된 SIGCHLD가 아니라 accept 함수를 통해서
    //생성된 자식 프로세스의 SIGCHLD이다. pid 값이 -1이 아닐때에만 처리한다.

    if( pid != -1)
    {
	for(i=0;i<CLIENT_NUM;++i)
	{
	    if(g_client_sockets[i].pid==pid)
	    {
		g_client_sockets[i].socket=-1;
		g_client_sockets[i].pid=-1;
		break;
	    }
	}

	printf("pid [%d] process terminated. status = %d\n",pid,status);
	printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n",--g_clientCnt);
    }
}

void sig_kill_server(int signum)
{
    int i=0;
    int cnt=0;
    int pid,status;
    printf("kill server\n");
   
    //close all clients
    for(i=0;i<CLIENT_NUM;++i)
    {
	if(g_client_sockets[i].socket !=-1 
		&& g_client_sockets[i].pid != -1)
	{
	    close(g_client_sockets[i]);
	    printf("%d\n",g_client_sockets[i].pid);

	    //SIGKILL or SIGTERM can kill child process safely.
	    kill(g_client_sockets[i].pid,SIGTERM);
	    cnt++;
	}
    }
    
    printf("총 %d 개의 클라이언트 종료됨.\n",cnt);

    //finally close server
    close(s_socket);
    exit(0);
}   

void sig_print(int signum)
{
    int i=0;

    for(i=0;i<CLIENT_NUM;++i)
    {
	puts("=================");
	printf("[%d]\n",i);
	printf("socket: %d\n",g_client_sockets[i].socket);
	printf("pid: %d\n",g_client_sockets[i].pid);
    } 
    puts("=================");
}

void do_service(int c_socket)
{
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

	/*
		!!!CRITICAL BUG POINT!!!
		g_ppid 를 0으로 초기화 하지 않으면 위의 printf 문을 수행하자마자 g_ppid 값이 변해버림. 알 수 없는 버그.
	*/
	//printf("3 %d\n",g_ppid);
	//printf("4 %d\n",getppid());

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
	    sprintf(sendBuffer,"[%s] strlen : %d",ppStr[1],strlen(ppStr[1]));
	    soc_write(c_socket,sendBuffer);
	    soc_freeCharPtrPtr(&ppStr,cntSplits);
	}
	else if(soc_msgcmp(rcvBuffer,"strcmp"))
	{
	    ppStr=soc_strsplit(rcvBuffer,dlim,&cntSplits);
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
	    close(c_socket);
	    break;
	}
	else if(soc_msgcmp(rcvBuffer,"kill server"))
	{
	    //자식 프로세스에서 raise를 쓰면 자식 프로세스에서만 시그널 발동할 것이기 때문에
	    //kill 함수를 써서 직접 해당 프로세스에게 시그널을 발생시키도록 했다.
	    //raise(SIG_KILL_SERVER);

	    soc_write(c_socket,"kill server commited");
	    //간단하게 getppid()함수를 써도 되는데 배운 pipe()함수를 활용해보기 위해서
	    //pipe를 통해 부모 프로세스값을 주고 받았다.
	    kill(g_ppid,SIG_KILL_SERVER);

	    //close(c_socket); 
	    //break;
	}
	else if(soc_msgcmp(rcvBuffer,"print socket"))
	{
	    soc_write(c_socket,"printed socket info to server");
	    kill(g_ppid,SIG_PRINT);
	}
	else
	{
	    //echo every msg
	    //여태까지 내가 만든 서버는 클라이언트가 보낸 메세지를 그대로 답변해주는 에코
	    //시스템이였다. 그러나 클라이언트가 ctrl+c로 종료하게되면 문제가 발상한다.
	    //클라이언트가 ctrl+c로 종료하면 서버한태 ctrl+c를 보내는거같은데
	    //이것을 서버가 클라이언트한태 에코해주면 무슨 이유때문인지 서버 프로그램이 무한
	    //루프에 빠지게된다. 서버에서 따로 ctrl+c를 처리하거나 클라이언트에서
	    //처리하는게 좋겠지만,다른 해결책이 떠오르기전까지는 일단 에코 시스템은 일시중단한다.
	    soc_write(c_socket,"무슨 말인지 모르겠습니다.");
	}

    }
}

