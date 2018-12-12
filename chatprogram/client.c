#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include "../lib/utils.h"

#define PORT 9000
#define IP_SERVER "127.0.0.1"

#define BUF_SIZ 10000
char buffer[BUF_SIZ];
char *g_nickname;

void *do_recv(void *data);
void *do_send(void *data);


int  main()
{
    //for socket
    int c_socket,s_socket;
    struct sockaddr_in c_addr;
    int len;
    int n;
    char rcvBuffer[BUF_SIZ+1];
    char sendBuffer[BUF_SIZ];

    //for thread
    pthread_t thread_send,thread_recv;
    int status_send,status_recv;

    c_socket=socket(PF_INET,SOCK_STREAM,0);

    memset(&c_addr,0,sizeof(c_addr));
    c_addr.sin_addr.s_addr=inet_addr(IP_SERVER);
    c_addr.sin_family=AF_INET;
    c_addr.sin_port=htons(PORT);

    if(connect(c_socket,(struct sockaddr *) &c_addr,sizeof(c_addr))==-1)
    {
	printf("can not connect\n");
	close(c_socket);
	return -1;
    }

    status_send=pthread_create(&thread_send,NULL,do_send,&c_socket);
    status_recv=pthread_create(&thread_recv,NULL,do_recv,&c_socket);
    pthread_join(thread_send,(void**)&status_send);
    pthread_join(thread_recv,(void**)&status_recv);

    close(c_socket);
    return 0;
}

void *do_recv(void *data)
{
    char rcvBuffer[BUF_SIZ]="";
    int n=0;
    int c_socket=*((int*)data);

    //recv
    while(1)
    {	
	memset(rcvBuffer,0,BUF_SIZ);

	n=read(c_socket,rcvBuffer,BUF_SIZ);
	puts("");

	if(n==0)
	{
	    printf("received FIN.\n");
	    close(c_socket);

	    if(g_nickname)
	    {
		free(g_nickname);
		g_nickname=NULL;
	    }
	    //서버로부터 FIN 을 받았다면 클라이언트 프로그램을 종료시킨다.
	    exit(0);
	}
	else if(n<0)
	{
	    printf("error code:%d\n",n);
	    close(c_socket);
	    if(g_nickname)
	    {
		free(g_nickname);
		g_nickname=NULL;
	    }
	    exit(-1);
	}

	rcvBuffer[n]='\0';
	printf("%s\n",rcvBuffer);	
	//fflush(stdout);
	printf("%s>",g_nickname);
	fflush(stdout);

	if(soc_msgcmp(rcvBuffer,"quit") || soc_msgcmp(rcvBuffer,"kill_server"))
	{
	    if(g_nickname)
	    {
		free(g_nickname);
		g_nickname=NULL;
	    }
	    close(c_socket);
	    exit(1);	    
	}
    }
}

void *do_send(void *data)
{
    int c_socket=*((int*)data);
    char sendBuffer[BUF_SIZ]="";

    //for login
    g_nickname=malloc(1);
    char buf[11]="";
    int bufsize=11, flag=0, size=0;

    //login
    printf("\nnickname : ");
    while( fgets(buf,bufsize - 1 ,stdin) )
    {
	size+=strlen(buf);
	if(buf[strlen(buf)-1]=='\n')
	{
	    size--;
	    flag=1;
	}
	g_nickname=realloc(g_nickname,size+1);
	strcat(g_nickname,buf);
	soc_write(c_socket,buf);

	if(flag==1)
	{
	    g_nickname[size]='\0';
	    break;
	}
	memset(buf, '\0', bufsize);
    }

    //printf("%s",g_nickname);

    //chat
    while(1)
    {
	memset(sendBuffer,0,BUF_SIZ);

	printf("\n%s> ",g_nickname);
	fgets(sendBuffer,BUF_SIZ,stdin);
	write(c_socket,sendBuffer,BUF_SIZ);
    }
}
