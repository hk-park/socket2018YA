#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";

main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int rcvLen;
	int itemp;
	
	char buffer[100];
	char rcvBuffer[100];
	char cmpBuff1[100];
	char cmpBuff2[100];

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
 	
	while(1) { // 클라이언트랑 통신하는 부분
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
	
		strcpy(buffer, "Server is connected\n");
		write(c_socket, buffer, strlen(buffer));
	
		while(1){
			strcpy(buffer, "\0");
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			
			if(strcmp(rcvBuffer, "안녕하세요")==0){ 
  				strcpy(buffer, "안녕하세요. 만나서 반가워요"); 
  				buffer[strlen(buffer)] = '\0'; 
  				write(c_socket, buffer, strlen(buffer)); 
  			} 
  			else if(strcmp(rcvBuffer, "이름이 머야?")==0){ 
  				strcpy(buffer, "내 이름은 아론이야."); 
  				buffer[strlen(buffer)] = '\0'; 
  				write(c_socket, buffer, strlen(buffer)); 
  			}
  			else if(strcmp(rcvBuffer, "몇 살이야?")==0){ 
  				strcpy(buffer, "나는 27살이야."); 
  				buffer[strlen(buffer)] = '\0'; 
  				write(c_socket, buffer, strlen(buffer)); 
  			} 
  			else if(strncasecmp(rcvBuffer, "strlen", 6)==0){ 
  				sprintf(buffer, "길이=%d", strlen(rcvBuffer)-7); 
  				write(c_socket, buffer, strlen(buffer)); 
  			} 
  			else if(strncasecmp(rcvBuffer, "strcmp", 6)==0){ 
  				strtok(rcvBuffer, " "); 
  				strcpy(cmpBuff1, strtok(NULL, " ")); 
  				strcpy(cmpBuff2, strtok(NULL, " ")); 
  				if(strcmp(cmpBuff1, cmpBuff2)==0){ 
  					strcpy(buffer, "문자열 일치(값=0)"); 
  				} 
  				else{ 
  					sprintf(buffer, "문자열 불일치(값=%d)", strcmp(cmpBuff1, cmpBuff2)); 
  				} 
  				write(c_socket, buffer, strlen(buffer)); 
  			} 

			else if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else{
				write(c_socket, "다시 시도해주세요.", 12);
			}
		}
		close(c_socket);
		if(strncmp(rcvBuffer, "kill server", 11)==0)
			break;
	}	
	close(s_socket);

	return 0;
}
