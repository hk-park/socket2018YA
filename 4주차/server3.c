#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 9000


// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
char rcvBuffer[100];
char buffer[100] = "My name is Hongkyu.\n";
char fromStr[100]="Hi, i'm Client\n";
char toStr[100]="Hi, i'm Server\n";

main( )
{
    int   c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int   len;
    int   n;
    s_socket = socket(PF_INET, SOCK_STREAM, 0);


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
	while(1)
	{
	    if((n=read(c_socket,rcvBuffer,sizeof(rcvBuffer)))<0)
	    {
		printf("can not read");
		return -1;
	    }

	    rcvBuffer[n]='\0';

	    printf("\nClient Msg : %s",rcvBuffer);
	    write(c_socket,rcvBuffer,n);

	    if(!strcmp(rcvBuffer,"quit\n"))
	    {
		close(c_socket);
		break;
	    }
	    else if(!strcmp(rcvBuffer,"kill server\n"))
	    {
		close(c_socket);
		close(s_socket);
		return 0;
	    }

	}

    }
    close(s_socket);
}

