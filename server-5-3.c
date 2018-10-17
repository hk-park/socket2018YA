#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000

int main(){
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	char sendBuffer[100], rcvBuffer[100];
	char str1[100], str2[100];
	int len, rcvLen;
	char* token;

    s_socket = socket(PF_INET, SOCK_STREAM, 0);

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP 할당
    s_addr.sin_family = AF_INET; //어떤 네트워크 프로토콜을 쓸지
    s_addr.sin_port = htons(PORT);

    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1){
			printf("Can not Bind\n");
 			return -1;
    	}

	if(listen(s_socket, 5) == -1) {
			printf("listen Fail\n");
			return -1;
    	}

   while(1){
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		printf("Clinet is connected\n");
			
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
		
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", strlen("kill server")) == 0)
				break;
			if(strncasecmp(rcvBuffer, "안녕하세요.", strlen("안녕하세요.")) == 0)
				strcpy(sendBuffer, "안녕하세요. 만나서 반가워요.\n");
			else if(strncasecmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")) == 0)
				strcpy(sendBuffer, "내 이름은 한종민이야.\n");
			else if(strncasecmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")) == 0)
				strcpy(sendBuffer, "나는 25살이야.\n");
			else if(strncasecmp(rcvBuffer, "strlen", strlen("strlen")) == 0){
				token = strtok(rcvBuffer, " ");
				token = strtok(NULL, "\0");			
				sprintf(sendBuffer, "%d\n", strlen(token));
			}
			else if(strncasecmp(rcvBuffer, "strcmp", strlen("strcmp")) == 0){
				strtok(rcvBuffer, " ");
				token = strtok(NULL, " ");
				strcpy(str1, token);
				token = strtok(NULL, " ");
				strcpy(str2, token);
				sprintf(sendBuffer, "%d\n", strcmp(str1, str2));
			}
			write(c_socket, sendBuffer, strlen(sendBuffer));
		}
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", strlen("kill server")) == 0)
			break;
		close(c_socket);
	}
	close(s_socket);
}
