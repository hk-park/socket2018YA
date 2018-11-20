#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
#define BUFSIZE 100
#define SLEEPTIME 1
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[BUFSIZE] = "Hi, I'm server\n";
char buffer2[BUFSIZE];
char *token;
char txt[BUFSIZE];
char str1[BUFSIZE];
char str2[BUFSIZE];
FILE *fp;
char buff[255];
char rcvBuffer[BUFSIZE];

main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	int readsize;
	char rcvBuffer[100];
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			memset(rcvBuffer,0,BUFSIZE);
			if(readsize=read (c_socket, rcvBuffer,sizeof (rcvBuffer))<0){
				return -1;
			}
			printf("Received Data From Client: %s\n",rcvBuffer);
			if(strcasecmp(rcvBuffer,"quit")==0){
				break;
			}
			else if(strcasecmp(rcvBuffer,"안녕하세요")==0){
				strcpy(buffer,"안녕하세요 만나서 반가워요");
			}
			else if(strcasecmp(rcvBuffer,"이름이 머야?")==0){
                                strcpy(buffer,"내 이름은 홍용호야");
                        }
			else if(strcasecmp(rcvBuffer,"몇 살이야?")==0){
                               strcpy(buffer,"나는 23살이야");
                        }
			else if(strncasecmp(rcvBuffer,"strlen",6)==0){
				token=strtok(rcvBuffer," ");
				token=strtok(NULL," ");
				strcpy(buffer2,token);
				sprintf(buffer,"%s, len : %d",buffer2,strlen(buffer2));
			}
			else if(strncasecmp(rcvBuffer, "strcmp",6)==0){
				token=strtok(rcvBuffer," ");
				strcpy(str1, strtok(NULL," "));
				strcpy(str2, strtok(NULL," "));
				if(strcmp(str1,str2)==0)
					sprintf(buffer,"0",str1,str2);
				else
					sprintf(buffer,"different",str1,str2);
			}
			n = strlen(buffer);
			write(c_socket,buffer,n);
		}
		close(c_socket);
	}	
	close(s_socket);
}
