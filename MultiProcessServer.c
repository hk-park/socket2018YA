#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#define PORT 9000
 
char buffer[BUFSIZ] = "Hi, I'm server\n";
void do_service(int c_socket); 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int pid;
	int   len;
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
		pid=fork();
		if(pid>0){//부모프로세스
			close(c_socket);
		}else if(pid==0){//자식프로세스(메시지를 주고받는 역할)
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}else{
			printf("[ERROR] fork failed\n");
			exit(0);
		}
	}	
	close(s_socket);
}

void do_service(int c_socket){
	int   n;
	int rcvLen;
	char rcvBuffer[BUFSIZ], *ptr, *ptr2;
while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
		
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			
			if(strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")) == 0){
				strcpy(buffer, "안녕하세요. 만나서 반가워요.");
			} else if(strncmp(rcvBuffer, "이름이 뭐야?", strlen("이름이 뭐야?"))==0){
				strcpy(buffer, "내 이름은 오승미야");
			} else if(strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?"))==0){
				strcpy(buffer, "나는 21살이야");
			} else{
				strcpy(buffer, "No data");
			}

			if(strncmp(rcvBuffer, "strlen ", 7) == 0){
				n = strlen(rcvBuffer)-7;
				sprintf(buffer, "%d", n);
			} else if(strncmp(rcvBuffer, "strcmp ", 7) == 0){
				ptr = strtok(rcvBuffer, " ");

				ptr = strtok(NULL, " ");
				ptr2 = strtok(NULL, " ");

				if(strcmp(ptr, ptr2) == 0){
				    	n = 0;
					sprintf(buffer, "%d", n);
				} else {
					n = -1;
					sprintf(buffer, "%d", n);
				}
			}

			write(c_socket, buffer, strlen(buffer));
		}
}
