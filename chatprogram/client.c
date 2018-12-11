#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include "../lib/utils.h"

#define PORT 9000
#define IP_SERVER "127.0.0.1"

#define BUF_SIZ 100
char buffer[BUF_SIZ];

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

    while(1)
    {	
	memset(rcvBuffer,0,BUF_SIZ);

	n=read(c_socket,rcvBuffer,BUF_SIZ);
	rcvBuffer[n]='\0';
	printf("%s\n",rcvBuffer);	

	if(soc_msgcmp(rcvBuffer,"quit") || soc_msgcmp(rcvBuffer,"kill_server"))
	{
	    break;
	}
    }
}

void *do_send(void *data)
{
    int c_socket=*((int*)data);
    char sendBuffer[BUF_SIZ]="";

    while(1)
    {
	memset(sendBuffer,0,BUF_SIZ);

	printf("\ntype> ");
	fgets(sendBuffer,BUF_SIZ,stdin);
	write(c_socket,sendBuffer,BUF_SIZ);
    }
}
