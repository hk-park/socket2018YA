#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "../lib/utils.h"

//DEFINEs
#define PORT 9000
#define BUF_SIZ 100

//GLOBALs
char rcvBuffer[BUF_SIZ];
char sendBuffer[BUF_SIZ];
char buffer[BUF_SIZ] = "My name is Hongkyu.\n";
char fromStr[BUF_SIZ]="Hi, i'm Client\n";
char toStr[BUF_SIZ]="Hi, i'm Server\n";


main( )
{
    int   c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int   len;
    int   n;
    s_socket = socket(PF_INET, SOCK_STREAM, 0);

    //for string split
    char dlim[]=" \n";
    char **ppStr=NULL;
    int cntSplits=0;

    //for FILE
    FILE *fp=NULL;
    char fileBuf[BUF_SIZ]="";
    char *fileStrBuild=NULL;
    int fileSize=0;

    //init socket
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
	printf("Can not Bind\n");
	return -1;
    }

    if(listen(s_socket, 5) == -1) {
	printf("listen Fail\n");
	return -1;
    }

    while(1) {
	len = sizeof(c_addr);
	c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);

	printf("client connected\n");

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
		return -1;
	    }

	    rcvBuffer[n]='\0';

	    printf("Client Msg : %s",rcvBuffer);

	    //process
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
		soc_freeCharPtrPtr(ppStr,cntSplits);
	    }
	    else if(soc_msgcmp(rcvBuffer,"strcmp"))
	    {
		ppStr=soc_strsplit(rcvBuffer,dlim,&cntSplits);
		sprintf(sendBuffer,"[%s] [%s] strcmp : %d",ppStr[1],ppStr[2],strcmp(ppStr[1],ppStr[2]));
		soc_write(c_socket,sendBuffer);
		soc_freeCharPtrPtr(ppStr,cntSplits);
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
		soc_freeCharPtrPtr(ppStr,cntSplits);
	    }
	    else if(soc_msgcmp(rcvBuffer,"exec"))
	    {
		ppStr=soc_strsplit(rcvBuffer," ", &cntSplits);
		//or 특징 : 첫번째 조건이 만족되면 이후 조건문들 수행 안함
		if(ppStr==NULL || system(ppStr[1])!=0)
		{
		    soc_write(c_socket,"!!exec error occured!!");
		    continue;
		}
		sprintf(sendBuffer,"[%s] processed",ppStr[1]);
		soc_write(c_socket,sendBuffer);
		soc_freeCharPtrPtr(ppStr,cntSplits);
	    }
	    else if(soc_msgcmp(rcvBuffer,"quit"))
	    {
		soc_write(c_socket,"quit commited");
		close(c_socket);
		break;
	    }
	    else if(soc_msgcmp(rcvBuffer,"kill server"))
	    {
		soc_write(c_socket,"kill server commited");
		close(c_socket);
		close(s_socket);
		return 0;
	    }
	    else
	    {
		//echo every msg
		soc_write(c_socket,rcvBuffer);
	    }


	}

    }
    close(s_socket);
}

