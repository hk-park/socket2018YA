#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h> 
#include <signal.h>
#include <sys/wait.h>
#define PORT 9000
#define BUFFERSIZE 100

char rcvBuffer[100];
char sendBuffer[100];
int current = 0;

void doService(int);
void sig_handler(int);

main()
{
	int pid;
	int   len;
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	signal(SIGCHLD, sig_handler);

	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if (bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}

	if (listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}

	while (1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		//클라이언트가 접속할 경우
		printf("새 클라이언트가 접속함: %d개 접속중\n", ++current);
		pid=fork();
		if(pid > 0){
			close(c_socket);
			continue;
		} else if(pid == 0){
			close(s_socket);
			doService(c_socket);
			close(c_socket);
			break;
		} else {
			printf("[Error]: fork failed\n");
			exit(0);
		}
	}
	close(s_socket);
}

void doService(int c_socket){
		int   n;
		int rcvLen;
		while (1) {
			// 스트링을 클라이언트에서 받고, 받은 스트링에 대해 \n 문자를 Null로 바꿔줌.
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			if (rcvBuffer[rcvLen - 1] = '\n')
				rcvBuffer[rcvLen - 1] = '\0';
			printf("클라이언트에서 받은 데이터[%d byte]: %s\n", rcvLen, rcvBuffer);
			// 길이와 받은 스트링을 출력한다.

			// 서버 종료 기능
			if ((!strncasecmp(rcvBuffer, "quit", 4))
				|| (!strncasecmp(rcvBuffer, "kill", 4)))
				break;

			// 채팅용 기능
			else if ((!strcasecmp(rcvBuffer, "hi"))
				|| (!strcmp(rcvBuffer, "안녕")))
				strcpy(sendBuffer, "Hello, Nice meet you.\n");
			else if ((!strncasecmp(rcvBuffer, "How old are you", 15))
				|| (!strcmp(rcvBuffer, "몇살이니")))
				strcpy(sendBuffer, "I am 3 days old.\n");
			else if ((!strncasecmp(rcvBuffer, "What is your name", 17))
				|| (!strcmp(rcvBuffer, "이름이 뭐니")))
				strcpy(sendBuffer, "I am HelloWorldServer.c!\n");

			// 함수 반환 기능
			else if (!strncasecmp(rcvBuffer, "strlen ", 7)) {
				// strlen 길이를 반환해준다
				sendBuffer[0] = '\0';
				strcpy(rcvBuffer, rcvBuffer + 7);
				sprintf(sendBuffer, "문자열 [%s]의 바이트는 %d\n", rcvBuffer, strlen(rcvBuffer));
			}
			else if (!strncasecmp(rcvBuffer, "strcmp ", 7)) {
				// strcmp 문자열 두개가 동일시 참, 아니면 거짓 반환
				char *cmpPtr;
				char take1[100], take2[100];
				int cmpResult;
				cmpPtr = strtok(rcvBuffer, " ");
				cmpPtr = strtok(NULL, " ");
				if (cmpPtr != NULL) //strtok로 아무 값을 못받을 경우
					strcpy(take1, cmpPtr);
				else
					strcpy(take1, "\0");
				cmpPtr = strtok(NULL, " ");
				if (cmpPtr != NULL) //strtok로 아무 값을 못받을 경우
					strcpy(take2, cmpPtr);
				else
					strcpy(take2, "\0");

				if (!strcmp(take1, take2))
					cmpResult = 0;
				else
					cmpResult = 1;

				sprintf(sendBuffer, "[%s]와 [%s]의 strcmp 결과값은 %d\n", take1, take2, cmpResult);




				/*
				char *token;
				char *str[3];
				int i=0;
				int cmpResult;
				token = strtok(rcvBuffer, " ");
				while(token != NULL){
						str[i++] = token;
						token = strtok(rcvBuffer, " ");
				}
				if(i < 3)
						sprintf(sendBuffer, "두 문자열이 필요합니다.\n");
				else if(str[1] == NULL || str[2] == NULL){
				printf("ERROR!\n");
						sprintf(sendBuffer, "받은 문자열이 없습니다.\n");}
				else{
						if(!strcmp(str[1], str[2]))
							cmpResult = 0;
						else
							cmpResult = 1;

						sprintf(sendBuffer, "[%s]와 [%s]의 strcmp 결과값은 %d\n", str[1], str[2], cmpResult);
				  }
				*/
				// 교수님 해설 소스코드
			}
			else if (!strncasecmp(rcvBuffer, "exec ", 5)) {
				// 서버에 명령어 전달
				strcpy(rcvBuffer, rcvBuffer + 5);
				if (!system(rcvBuffer))
					sprintf(sendBuffer, "성공함: %s\n", rcvBuffer);
				else
					sprintf(sendBuffer, "실행실패: %s\n", rcvBuffer);
			}
			else if (!strncasecmp(rcvBuffer, "fopen ", 6)) {
				// 파일입출력 기
				strcpy(rcvBuffer, rcvBuffer + 6);
				FILE *fp;
				char buffer[BUFFERSIZE];

				if ((fp = fopen(rcvBuffer, "r")) == NULL) {
					sprintf(sendBuffer, "%s 열기 실패 \n", rcvBuffer);
				}
				else {// 파일열기 성공시
					strcpy(buffer, "파일 열기 성공\n");
					n = strlen(buffer);
					write(c_socket, buffer, n);
					//성공결과를 클라이언트에 보내줌

					while (fgets(buffer, 100, (FILE *)fp)) {
						n = strlen(buffer);
						write(c_socket, buffer, n); //매 데이터를 클라이언트에 보냄
					}
					strcpy(sendBuffer, " * 전송끝 * \n");
				}
			}
			else
				sprintf(sendBuffer, "%s는 올바른 명령어가 아닙니다.\n", rcvBuffer);


			// 스트링을 클라이언트로 보냄
			n = strlen(sendBuffer);
			write(c_socket, sendBuffer, n);
			rcvBuffer[0] = '\0';
		}
}

void sig_handler(int signo){
	int pid;
	int status;
	pid = wait(&status);
	//printf("pid[%d] terminated, status = %d\n", pid, status);
	current--;
	printf("클라이언트 종료됨: %d개 접속중\n", current);
}
