#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

#define PORT 9000
#define BUFSIZE 100
char buffer[100] = "Hi, I'm server\n";

char *response(char*rcvBuffer);
void do_service(int c_socket);

int pid;
int   c_socket, s_socket;
struct sockaddr_in s_addr, c_addr;
int   len;
int   n;
int rcvLen;
char rcvBuffer[BUFSIZE];
char* sep=" ";
char* token;
int i = 0;
char str[3][BUFSIZE];
char command[BUFSIZE];
FILE *fp;
char fileBuffer[BUFSIZE];

void sig_handler();

main( )
{
	signal(SIGCHLD, sig_handler);	
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
		if(pid = fork()>0){
				close(c_socket);
				continue;
		}
		else if(pid==0){
			while(1){
				close(s_socket);
				do_service(c_socket);
				exit(0);
			}
		}
		else {
			printf("error!!");
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
		break;
	}	
	close(s_socket);
}

void do_service(int c_socket){  
	while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);

			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else if(!strncmp(rcvBuffer, "안녕하세요.", strlen("안녕하세요.")))
                                strcpy(buffer, "안녕하세요. 만나서 반갑습니다.\n");
                        else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
                                strcpy(buffer, "내 이름은 이호은 이야\n");
                        else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
                                strcpy(buffer, "내 나이는 21살 이야\n");
                        else if(!strncmp(rcvBuffer, "strlen ", strlen("strlen"))){
				sprintf(rcvBuffer, "%d", strlen(rcvBuffer)-7);
                                strcpy(buffer, rcvBuffer);
                        }
                        else if(!strncmp(rcvBuffer, "strcmp", strlen("strcmp"))) {
				memset(str, 0, sizeof(str));
				token = strtok(rcvBuffer, sep);
				i=0;
				while(token){
					strcpy(str[i], token);
					token = strtok(NULL, sep);
					i++;
				}
				if(i==3)sprintf(buffer, "문자열 비교 결과 : %d", strcmp(str[1], str[2]));
				else sprintf(buffer, "비교할 인자가 많거나 적습니다... 두개만...");
			}
			else if(!strncmp(rcvBuffer, "readfile", strlen("readfile"))){
				memset(str, 0, sizeof(str));
				token = strtok(rcvBuffer, sep);
				i=0;
				while(token){
					strcpy(str[i], token);
					token = strtok(NULL, sep);
					i++;
				}
				fp = fopen(str[1], "r");
				if(fp){
					while(fgets(fileBuffer, BUFSIZE, (FILE *)fp)){
					sprintf(buffer, "%s", fileBuffer);
					n = strlen(buffer);
                        		write(c_socket, buffer, n);
					}
				} else printf("파일을 찾을 수 없습니다.\n");
				continue;
			}
			else if(!strncmp(rcvBuffer, "exec", strlen("exec"))){
				memset(command, 0, sizeof(command));
				token = strtok(rcvBuffer, sep);
				strcpy(command,strtok(NULL, "\0"));
				printf("command : %s\n", command);
				int result = system(command);
				if(!result) sprintf(buffer, "%s", "명령어가 정상적으로 수행되었습니다.\n");
			
				else sprintf(buffer, "%s", "몀령어 수행을 실패하였습니다.\n");				
			}
                        else{
				sprintf(buffer, "무슨 말인지 모르겠습니다.");
                        }
	               n = strlen(buffer);
	               write(c_socket, buffer, n);
	}
}

void sig_handler(){
	int pid;
	int status;
	pid = wait(&status);
	printf("PID[%d] process terminated.status = %d\n", pid ,status);
}
