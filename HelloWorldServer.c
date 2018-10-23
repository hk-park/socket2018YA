#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000

char buffer[100] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100];
	char* sep=" ";
	char* token;
	int i = 0;
	char str[3][50];
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
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
		
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else if(strncmp(rcvBuffer, "안녕하세요.", 16) == 0){
                                strcpy(buffer, "안녕하세요. 만나서 반갑습니다.\n");
                        }
                        else if(strncmp(rcvBuffer, "이름이 머야?", 17) == 0){
                                strcpy(buffer, "내 이름은 이호은 이야\n");
                        }
                        else if(strncmp(rcvBuffer, "몇 살이야?", 16) == 0){
                                strcpy(buffer, "내 나이는 21살 이야\n");
                        }
                        else if(strncmp(rcvBuffer, "strlen ", 7) == 0){
				sprintf(rcvBuffer, "%d", strlen(rcvBuffer)-7);
                                strcpy(buffer, rcvBuffer);
                        }
                        else if(strncmp(rcvBuffer, "strcmp", 6) == 0) {
				token = strtok(rcvBuffer, sep);
				i=0;
				while(token){
					strcpy(str[i], token);
					token = strtok(NULL, sep);
					i++;
				}
				sprintf(rcvBuffer, "%d", strcmp(str[1], str[2]));
			}
                        else{
                                strcpy(buffer, rcvBuffer);
                        }
                        n = strlen(buffer);
                        write(c_socket, buffer, n);
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
