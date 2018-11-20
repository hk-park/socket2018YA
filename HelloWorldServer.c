#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define COM 5
#define BUFSIZE 100
#define IPADDR "127.0.0.1"

void sendToClient(int c_socket, char *buffer){
	write(c_socket, buffer, strlen(buffer));	
}

main( )
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, check, pid;
	int rcvLen;
	char * command, *token;
	char buffer[BUFSIZE];
	char rcvBuffer[BUFSIZE];
	char tok[BUFSIZE];
	char tokCmp[BUFSIZE];
	FILE *fp;

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

			if(!strncasecmp(rcvBuffer, "quit", strlen("quit")) || !strncasecmp(rcvBuffer, "kill server", strlen("kill server")))
				break;
			else if(!strncasecmp(rcvBuffer, "안녕하세요.", strlen("안녕하세요.")))
				strcpy(buffer, "안녕하세요. 만나서 반가워요.");		
			else if(!strncasecmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
				strcpy(buffer, "내 이름은 김서원이야.");		
			else if(!strncasecmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer, "나는 26살이야...");
			else if(!strncasecmp(rcvBuffer, "strlen", strlen("strlen")))
				sprintf(buffer, "%d", strlen(rcvBuffer)-7);
			else if(!strncasecmp(rcvBuffer, "strcmp", strlen("strcmp"))){
				strtok(rcvBuffer, " ");
				strcpy(tok, strtok(NULL, " "));
				strcpy(tokCmp, strtok(NULL, " "));
				if(!strcmp(tok, tokCmp))
					sprintf(buffer, "문자열 같음(0)");
				else
					sprintf(buffer, "문자열 같지 않음(1)");
			}
			else if(!strncasecmp(rcvBuffer, "readfile", strlen("readfile"))){
				strtok(rcvBuffer, " ");
				strcpy(tok, strtok(NULL, " "));

				fp = fopen(tok, "r");	
				if(fp){
					while(fgets(buffer, BUFSIZE, (FILE *) fp))
						sendToClient(c_socket, buffer);
					continue;
				} 
				else sprintf(buffer, "%s", "파일이 없습니다.");
			}
			else if(!strncasecmp(rcvBuffer, "exec", strlen("exec"))){
				token = strtok(rcvBuffer, " ");
				command = strtok(NULL, "\0");

				if(!system(command))
					sprintf(buffer, "%s", "command is executed.");
				else
					sprintf(buffer, "%s", "command failed.");
			}
			else
				strcpy(buffer, "답변할 수 없는 질문입니다.");

			sendToClient(c_socket, buffer);
		}
		printf("Client is logout.\n");
		close(c_socket);

		if(strncasecmp(rcvBuffer, "kill server", strlen("kill server")) == 0)
			break;
	}	
	printf("Bye.\n");
	close(s_socket);
}
