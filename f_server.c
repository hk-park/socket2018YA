#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
char helloBuffer[100] = "챗봇: 안녕하세요. 만나서 반가워요!.\n";
char hiBuffer[100] = "네, 부르셨어요?\n";
char nameBuffer[100] = "챗봇 : 제 이름은 챗봇이에요^^.\n";
char ageBuffer[100] = "챗봇 : 저는 1살이에요^^.\n";


char str1[100] = "";
char str2[100] = "";

int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n, n2, nCount;
	int pid;
	int rcvLen;
	char rcvBuffer[100];


void do_service(int c_socket){

	
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
					sprintf(buffer, "두 문자가 일치합니다.");
				else
					sprintf(buffer, "두 문자가 일치하지 않습니다.");

				n2 = strlen(buffer);
				write(c_socket,buffer,n2);
	

			}



			if(strncasecmp(rcvBuffer, "안녕하세요.", strlen("안녕")) == 0)
			{
				n2 = strlen(helloBuffer);
				write(c_socket, helloBuffer, n2);
				
			}	

			if(strncasecmp(rcvBuffer, "챗봇~", strlen("챗봇")) == 0)
			{
				n2 = strlen(hiBuffer);
				write(c_socket, hiBuffer, n2);
				
			}


			if(strncasecmp(rcvBuffer, "이름이 뭐니?", strlen("이름이 뭐")) == 0)
			{
				n2 = strlen(nameBuffer);
				write(c_socket, nameBuffer, n2);
				
			}

			if(strncasecmp(rcvBuffer, "몇 살이니?", strlen("몇 살이")) == 0)
			{
				n2 = strlen(ageBuffer);
				write(c_socket, ageBuffer, n2);
				
			}
		

			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			n = strlen(buffer);
			write(c_socket,"\n",5);
			
		}
		close(c_socket);
		//if(!strncasecmp(rcvBuffer, "kill server", 11))
		//	break;

}


main( )
{
	
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

		if((pid = fork( )) > 0) {// 부모 프로세스
		 // 다른 클라이언트의 요청 접수
		   close(c_socket);
		   continue;

		} else if(pid == 0) {// 자식 프로세스:
		   // 클라이언트의 요청 처리
		   close(s_socket);
		   do_service(c_socket);
		   exit(0);
		}
 	
	
	}	
	close(s_socket);
}



