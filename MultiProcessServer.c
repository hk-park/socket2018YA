#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
#define BUFSIZE 10000

char buffer[BUFSIZE] = "Hi, I'm server\n";
char rcvBuffer[BUFSIZE];

void function (int c_socket);
void sig_handler(int signo);

main( )
{
	int   len;
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int pid;
	signal(SIGCHLD, sig_handler);
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
		pid = fork();
		//부모인 경우
		if(pid>0){
			close(c_socket);
		//자식으로 분열 후
		}else if(pid == 0){
			close(s_socket);
			function(c_socket);
			exit(0);			
		}else{
			printf("[ERROR] fork failed\n");
			exit(0);
		}
	}	
	close(s_socket);
}
void function(int c_socket){
	int   n;
	int readSize;
	char *read_text;
	char tempBuf[BUFSIZE];

	while(1){
				memset(rcvBuffer, 0, BUFSIZE);
				if((readSize = read(c_socket, rcvBuffer, sizeof(rcvBuffer)))<0){
					return -1;
				}
				printf("Received Data From Client: %s\n", rcvBuffer);
				if(strcasecmp(rcvBuffer, "quit")== 0 || strcasecmp(rcvBuffer, "kill server")==0){
					break;
				}else if(strcasecmp(rcvBuffer, "kill server") == 0){
					break;
				}else if(strncasecmp(rcvBuffer, "readfile",8)==0){
					read_text = strtok(rcvBuffer, " ");
					read_text = strtok(NULL, " ");
					FILE *fp = fopen(read_text, "r");
					char tempStr[BUFSIZE];
					memset(buffer, 0, BUFSIZE);
					while(fgets(tempStr, BUFSIZE, (FILE *)fp)){
						strcat(buffer, tempStr);
					}
					fclose(fp);
				}

				printf("[%s] received\n", rcvBuffer);
				n = strlen(buffer);
				write(c_socket, buffer, n);
	}
}
void sig_handler(int signo){
	int pid;
	int status;
	pid = wait(&status);
	printf("pid[%d] process terminated.status = %d\n", pid, status);
}
