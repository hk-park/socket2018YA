#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define BUFSIZE 100

int main(){
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	char sendBuffer[100], rcvBuffer[100];
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
				char *token;
				char *str[3];
				int i = 0;
				token = strtok(rcvBuffer, " ");
				while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
				}
				if(i<3)
					sprintf(sendBuffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
				else if(!strcmp(str[1], str[2])) //같은 문자열이라면,
					sprintf(sendBuffer, "%s와 %s는 같은 문자열입니다.", str[1], str[2]);
				else
					sprintf(sendBuffer, "%s와 %s는 다른 문자열입니다.", str[1], str[2]);
			}
			else if(!strncasecmp(rcvBuffer, "readfile", strlen("readfile"))){
				char *token;
				char *str[2];
				int i = 0;
				token = strtok(rcvBuffer, " ");
				while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
				}
				if(i<2)
					sprintf(sendBuffer, "파일을 읽기 위해서는 readfile <파일명> 형태로 입력하세요.");
				FILE *fp = fopen(str[1], "r");
				if(fp){
					char tempBuffer[BUFSIZE];
					while(fgets(tempBuffer, BUFSIZE, (FILE *)fp))
						strcat(sendBuffer, tempBuffer);
				fclose(fp);
				} else
					sprintf(sendBuffer, "파일이 없습니다.\n");
			}
			write(c_socket, sendBuffer, strlen(sendBuffer));
		}
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", strlen("kill server")) == 0)
			break;
		close(c_socket);
	}
	close(s_socket);
}
