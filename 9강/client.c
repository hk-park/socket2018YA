#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include "../lib/utils.h"

#define PORT 9000
#define IP_SERVER "127.0.0.1"

#define BUF_SIZ 100
char buffer[BUF_SIZ];

int  main()
{
    int c_socket,s_socket;
    struct sockaddr_in c_addr;
    int len;
    int n;
    char rcvBuffer[BUF_SIZ+1];
    char sendBuffer[BUF_SIZ];

    //for read huge file from server
    int readBytes=0;
    char **ppStr=NULL;
    int arrCnt=0;

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

    while(1)
    {
	printf("\ntype> ");
	fgets(sendBuffer,BUF_SIZ,stdin);
	write(c_socket,sendBuffer,BUF_SIZ);

	n=read(c_socket,rcvBuffer,BUF_SIZ);
	rcvBuffer[n]='\0';
	printf("server echo : %s\n",rcvBuffer);	

	if(soc_msgcmp(rcvBuffer,"readfile"))
	{
	    puts("");
	    ppStr=soc_strsplit(rcvBuffer," ",&arrCnt);
	    readBytes=atoi(ppStr[1]);
	    soc_write(c_socket,"ready to go");

	    while(readBytes>0)
	    {
		n=read(c_socket,rcvBuffer,BUF_SIZ);
		rcvBuffer[n]='\0';
		printf("%s",rcvBuffer);
		readBytes-=n;
	    }
	    puts("");
	    soc_freeCharPtrPtr(&ppStr,arrCnt);
	}
	else if(soc_msgcmp(rcvBuffer,"quit") || soc_msgcmp(rcvBuffer,"kill server"))
	{
	    break;
	}
    }
    puts("hi");
    close(c_socket);
    puts("hi2");
    return 0;
}
