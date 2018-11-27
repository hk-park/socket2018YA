#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h> //exit위해 
#include <signal.h> //signal
#include <sys/wait.h> //wait 함수

#define PORT 9000
#define BUFSIZE 10000
char Hbuffer[BUFSIZE] = "안녕하세요. 만나서 반가워요\n";
char Nbuffer[BUFSIZE] = "내 이름은 지윤이야.\n";
char Abuffer[BUFSIZE] = "나는 22살이야.\n";
char nothing[BUFSIZE] = "XXXXXXXX \n";

void do_service(int c_socket);
void sig_handler(int signo);
int ClientNum=0;

main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int pid;
	int len;
	
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
		printf("Client is connected\n");
		ClientNum++;
		printf("현재 %d개의 클라이언트가 접속하였습니다.\n",ClientNum);
		pid = fork();
		if(pid > 0) { //부모 프로세스가 해야 할 일
			close(c_socket); //부모 프로세스는 메세지를 주고 받을 일 없음
		} else if (pid == 0) { //자식 프로세스가 해야 할 일
			close(s_socket);
			do_service(c_socket);
			exit(0);
		} else { //실패
			printf("[ERROR] fork failed\n");
			exit(0);
		}
		close(c_socket);
	}	
	close(s_socket);
}

void do_service(int c_socket) {
	//클라이언트의 요청 처리 루틴
	FILE *fp;
	int i, x;
	char  *ptr,  *ptr1, *ptr2, buffer[BUFSIZE], fbuffer[BUFSIZE];
	int   n;
	int rcvLen;
	char rcvBuffer[BUFSIZE];
	while(1){
			char *token;
			char *str[3];
			int i = 0;
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			buffer[0] = '\0';
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			if(!strncasecmp(rcvBuffer, "안녕하세요.", 6)){
				n = strlen(Hbuffer);
				write(c_socket, Hbuffer, n);
			}
			else if(!strncasecmp(rcvBuffer, "이름이 뭐야?", 7)){
				n = strlen(Nbuffer);
				write(c_socket, Nbuffer, n);
			}
			else if(!strncasecmp(rcvBuffer, "몇 살이야?", 6)){
				n = strlen(Abuffer);
				write(c_socket, Abuffer, n);
			}
			else if(!strncasecmp(rcvBuffer, "strlen", 6)){
				x = strlen(rcvBuffer)-7;			
				sprintf(buffer,"길이 : %d",x);
				write(c_socket, buffer, strlen(buffer));
			}
			else if(!strncasecmp(rcvBuffer, "strcmp", 6)){
				ptr = strtok(rcvBuffer, " ");
				ptr = strtok(NULL, " ");
				ptr1 = ptr;
				ptr = strtok(NULL, " ");
				ptr2 = ptr;
				x = strcmp(ptr1, ptr2);
				sprintf(buffer,"결과 : %d",x);
				write(c_socket, buffer, strlen(buffer));
			}
			else if(!strncasecmp(rcvBuffer, "readfile", 8)){
				ptr = strtok(rcvBuffer, " ");
				ptr = strtok(NULL, " ");
				ptr1 = ptr;
				fp = fopen(ptr1, "r");
				if(fp){
					while(fgets(fbuffer, BUFSIZE, (FILE *)fp))
							strcat(buffer, fbuffer);
				}
				else{
					strcpy(buffer, "존재하지 않는 파일");
				}
				fclose(fp);	
				write(c_socket, buffer, strlen(buffer));			
			}
			else if(!strncasecmp(rcvBuffer, "exec", 4)){
				ptr = strtok(rcvBuffer, " ");
				ptr = strtok(NULL, "NULL");
				ptr1 = ptr;			
				int ret = system(ptr1);
				if(!ret)
					sprintf(buffer, "<%s> command Success!!\n", ptr);
				else
					sprintf(buffer, "<%s> command Failed!!\n", ptr);
				write(c_socket, buffer, strlen(buffer));
			}
			else{
				n = strlen(nothing);
				write(c_socket, nothing, n);
			}
		}
}

void sig_handler(int signo)
{
	int pid;
	int status; //종료 상태 알아보기(정상, 비정상)
	
	pid = wait(&status); //자식 프로세스가 완전히 종료될 때까지 기다려주는 함수
	printf("pid[%d] process terminated status = %d\n", pid, status);
	
	ClientNum--;
	printf("1개의 클라이언트가 접속되어 %d개의 클라이언트가 접속되어 있습니다.\n",ClientNum);
}
