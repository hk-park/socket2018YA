#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
char buffer2[100] = "챗봇의 말 : 안녕하세요. 만나서 반가워요!\n";
char buffer3[100] = "제 이름은 서현봇이예요.\n";
char buffer4[100] = "저는 22살이예요.\n";

char ptr1[100] = "";
char ptr2[100] = "";


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
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);




			if(strncasecmp(rcvBuffer, "strlen", strlen("strlen")) == 0)
			{
				sprintf(ptr1, "글자 길이 : %d", strlen(rcvBuffer)-7);
				n2 = strlen(ptr1);
				write(c_socket,ptr1,n2);
			
								
			}

			if(strncasecmp(rcvBuffer, "strcmp", strlen("strcmp")) == 0)
			{

				strtok(rcvBuffer, " ");
				strcpy(ptr1, strtok(NULL, " "));
				strcpy(ptr2, strtok(NULL, " "));
				if(strcmp(ptr1, ptr2) == 0)
					sprintf(buffer, "같은 문자입니다.");
				else
					sprintf(buffer, "같은 문자가 아닙니다.");

				n2 = strlen(buffer);
				write(c_socket,buffer,n2);
	

			}



			if(strncasecmp(rcvBuffer, "안녕하세요.", strlen("안녕하세요")) == 0)
			{
				n2 = strlen(buffer2);
				write(c_socket, buffer2, n2);
				
			}	


			if(strncasecmp(rcvBuffer, "이름이 뭐야?", strlen("이름이 뭐야")) == 0)
			{
				n2 = strlen(buffer3);
				write(c_socket, buffer3, n2);
				
			}

			if(strncasecmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야")) == 0)
			{
				n2 = strlen(buffer4);
				write(c_socket, buffer4, n2);
				
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



