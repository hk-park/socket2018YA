#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
// #define PORT 10000
  
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
// char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
void sig_handler(int signo);
void do_service(int c_socket);

int	c_socket, s_socket;
struct	sockaddr_in s_addr, c_addr;
int	len;
int	rcvLen;
int	rcvstrlen;
char	rcvBuffer[100];
int	n;
int numClient=0;


main( ) {
	int pid;
	signal(SIGCHLD, sig_handler);

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
		printf("Client is connected.\n");
		numClient++;
		printf("현재 %d개의 클라이언트가 접속하였습니다.\n", numClient);
		pid = fork();
		if(pid !=  0) {
			close(c_socket);
			continue;
		} else if (pid == 0) {
			close(s_socket);
			do_service(c_socket);
			exit(0);	
		} else {
			printf("[ERROR] fork failed.\n");
			exit(0);
		}
	}
	close(s_socket);
}







void do_service(int c_socket){
	char *compare;
	char *str[3];
	int i=0;

	while(1) {
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';

			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			if(strncasecmp(rcvBuffer,"안녕하세요.", 11) == 0) {
    	                        printf("%s 만나서 반가워요.\n", rcvBuffer);
   	                        read(c_socket, rcvBuffer, sizeof(rcvBuffer));
       	                }
    	                if(strncasecmp(rcvBuffer, "이름이 뭐야?", 12) == 0) {
                                printf("내 이름은 노수현이야.\n");
                                read(c_socket, rcvBuffer, sizeof(rcvBuffer));
                        }
                        if(strncasecmp(rcvBuffer,"몇 살이야?", 10) == 0) {
                                printf("나는 22살이야.\n", rcvBuffer);
                                read(c_socket, rcvBuffer, sizeof(rcvBuffer));
                        }
                        if(strncasecmp(rcvBuffer, "strlen ", 7) == 0) {
                                rcvstrlen=rcvLen-7;
                                printf("%d\n", rcvstrlen);
                                read(c_socket, rcvBuffer, sizeof(rcvBuffer));
                        }		
			if(strncasecmp(rcvBuffer, "strcmp ", 7) == 0) {
				i=0;
				compare=strtok(rcvBuffer, " ");
				while (compare != NULL) {
					str[i++] = compare;
					compare = strtok(NULL, " ");
				}
				if(i<3)	sprintf(buffer, "두개의 문자열이 필요합니다.");
				else if(!strcmp(str[1],str[2]))	sprintf(buffer, "%s 와 %s는 같은 문자열입니다.", str[1], str[2]);
				else	sprintf(buffer, "%s 와 %s 는 다른 문자열입니다.", str[1], str[2]);
			}
	
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}
}

void sig_handler(int signo) {
	int pid;
	int status;
	pid = wait(&status); //자식 프로세스가 종료될 때까지 기다려주는 함수
	printf("pid[%d] process terminated. status = %d\n", pid, status);
	numClient--;
	printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n", numClient);
}

