#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define IPADDR "127.0.0.1"

void sendToClient(int c_socket, char *buffer){
	write(c_socket, buffer, strlen(buffer));	
}

main( )
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, check;
	int rcvLen;
	char buffer[100];
	char rcvBuffer[100];
	char tok[100];
	char tokCmp[100];
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = inet_addr(IPADDR);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind.\n");
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) {
		printf("listen Fail.\n");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		printf("Client is login.\n");

		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("Client >> %s\n", rcvBuffer);

			if(!strncasecmp(rcvBuffer, "quit", 4) || !strncasecmp(rcvBuffer, "kill server", 11))
				break;
			else if(!strncasecmp(rcvBuffer, "안녕하세요.", 16))
				strcpy(buffer, "안녕하세요. 만나서 반가워요.");		
			else if(!strncasecmp(rcvBuffer, "이름이 머야?", 17))
				strcpy(buffer, "내 이름은 김서원이야.");		
			else if(!strncasecmp(rcvBuffer, "몇 살이야?", 14))
				strcpy(buffer, "나는 26살이야...");
			else if(!strncasecmp(rcvBuffer, "strlen ", 7))
				sprintf(buffer, "%d", strlen(rcvBuffer)-7);
			else if(!strncasecmp(rcvBuffer, "strcmp ", 7)){
				strtok(rcvBuffer, " ");
				strcpy(tok, strtok(NULL, " "));
				strcpy(tokCmp, strtok(NULL, " "));
				if(!strcmp(tok, tokCmp))
					sprintf(buffer, "문자열 같음(0)");
				else
					sprintf(buffer, "문자열 같지 않음(1)");
			}
			else
				strcpy(buffer, "답변할 수 없는 질문입니다.");

			sendToClient(c_socket, buffer);
		}
		printf("Client is logout.\n");
		close(c_socket);

		if(strncasecmp(rcvBuffer, "kill server", 11) == 0)
			break;
	}	
	printf("Bye.\n");
	close(s_socket);
}

