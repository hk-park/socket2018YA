#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 9000
#define BUFSIZE 10000

void do_service(int c_socket);
void sig_handler();

char sendBuffer[BUFSIZE], rcvBuffer[BUFSIZE];
int c_socket, s_socket;
struct sockaddr_in s_addr, c_addr;
int pid;
int len, rcvLen;
int client_count = 0;
char *token;

int main(){
	signal(SIGCHLD, sig_handler);
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
		pid = fork();
		if(pid > 0){ //부모 프로세스
			//다른 클라이언트의 요청 접수
			printf("Clinet is connected\n");
			printf("현재 %d개의 클라이언트가 접속하였습니다.\n",++client_count);
			close(c_socket);
			continue;
		}
		else if(pid == 0){ //자식 프로세스
			//클라이언트의 요청 처리
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}
		else
		{
			printf("[ERROR] fork failed\n");
			exit(0);
		}
	}
}

void do_service(int c_socket){
	while(1){
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		printf("Received Data From Client : %s\n", rcvBuffer);
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
			break;
		if(rcvLen < 0)
				exit(0); //do_service()함수를 실행함. read()함수에서 클라이언트가 메시지를 보낼때까지 대기
		rcvBuffer[rcvLen] = '\0';
		
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
				sprintf(sendBuffer, "%s와 %s는 같은 문자열입니다.\n", str[1], str[2]);
			else
				sprintf(sendBuffer, "%s와 %s는 다른 문자열입니다.\n", str[1], str[2]);
		}
		else if(!strncasecmp(rcvBuffer, "readfile ", 9)){
			char *token;
			char *file[2];
			int i = 0;
			token = strtok(rcvBuffer, " ");
			while(token != NULL){
				file[i++] = token;
				token = strtok(NULL, " ");
			}
			if(i<2)
				sprintf(sendBuffer, "파일을 읽기 위해서는 readfile <파일명> 형태로 입력하세요.\n");
			FILE *fp = fopen(file[1], "r");
			if(fp){
				char tempBuffer[BUFSIZE];
				memset(sendBuffer, 0, BUFSIZE);
				while(fgets(tempBuffer, BUFSIZE, (FILE *)fp))
					strcat(sendBuffer, tempBuffer);
			fclose(fp);
			} else 
				sprintf(sendBuffer, "파일이 없습니다.\n");
		}
		else if(!strncasecmp(rcvBuffer, "exec ",5)){
			char *token;
			char *command;
			token = strtok(rcvBuffer, " ");
			command = strtok(NULL, "\0");
			if(command)
				sprintf(sendBuffer, "exex <명령어> 형태로 입력하세요.\n");
			int result = system(command);
			if(!result)
				sprintf(sendBuffer, "[%s] command is executed\n",command);
			else
				sprintf(sendBuffer, "[%s] command failed\n",command);
		}
		write(c_socket, sendBuffer, strlen(sendBuffer));
	}
	close(s_socket);
}

void sig_handler(int signo){
	int pid;
	int status;
	pid = wait(&status);	//자식 프로세스가 종료될 때까지 기다려주는 함수
	printf("pid[%d] process terminated. status = %d\n",pid, status);
	printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n", --client_count);
}

