#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
char buffer[100] = "Hi, I'm server\n";
char bufferHi[100] = "챗봇 : 안녕하세요. 만나서 반가워요.\n";
char bufferName[100] = "챗봇 : 내 이름은 챗봇 [ 준원 ]이야.\n";
char bufferAge[100] = "챗봇 : 나는 21살이야.\n";
char bufferchat[100] = "왜 그러세요? \n";
char str1[100] = "";
char str2[100] = "";


main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n, n2, nCount;
	int rcvLen;
	char rcvBuffer[100];
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
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);




			if(strncasecmp(rcvBuffer, "strlen", strlen("strlen")) == 0)
			{
				sprintf(str1, "%d", strlen(rcvBuffer)-7);
				n2 = strlen(str1);
				write(c_socket,str1,n2);
			
								
			}

			if(strncasecmp(rcvBuffer, "strcmp", strlen("strcmp")) == 0)
			{

				strtok(rcvBuffer, " ");
				strcpy(str1, strtok(NULL, " "));
				strcpy(str2, strtok(NULL, " "));
				if(strcmp(str1, str2) == 0)
					sprintf(buffer, "문장이 일치해요");
				else
					sprintf(buffer, "문장이 일치하지 않아요.");

				n2 = strlen(buffer);
				write(c_socket,buffer,n2);
	

			}



			if(strncasecmp(rcvBuffer, "안녕", strlen("안녕")) == 0)
			{
				n2 = strlen(bufferHi);
				write(c_socket, bufferHi, n2);
				
			}
			if(strncasecmp(rcvBuffer, "챗봇아", strlen("챗봇")) == 0)
			{
				n2 = strlen(bufferchat);
				write(c_socket, bufferchat, n2);
				
			}		


			if(strncasecmp(rcvBuffer, "넌 이름이 뭐야", strlen("넌 이름이 뭐야")) == 0)
			{
				n2 = strlen(bufferName);
				write(c_socket, bufferName, n2);
				
			}

			if(strncasecmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야")) == 0)
			{
				n2 = strlen(bufferAge);
				write(c_socket, bufferAge, n2);
				
			}
		

			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			n = strlen(buffer);
			write(c_socket,"\n",5);
			
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}



