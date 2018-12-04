#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

// 2-1. 서버 프로그램이 사용하는 포트
#define PORT 9000
#define BUFSIZE 10000
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정

void *do_service(void* val);

char buffer[BUFSIZE] = "Hi, I'm Server.\n";
char nothing[BUFSIZE] = "아무것도 아님\n";

int main()
{
	int c_socket, s_socket, rcvLen, len;		// 변수 선언
	struct sockaddr_in s_addr, c_addr;	// 클라이언트에서 보낸 데이터 받음
	char rcvBuffer[BUFSIZE], *ptr,  *ptr1, *ptr2, buffer[BUFSIZE];				// 소켓 통신 정보 (구조체)	

	pthread_t pthread;
	int thread_id;

	s_socket = socket(PF_INET, SOCK_STREAM, 0);			// 서버 소켓 생성	

	memset(&s_addr, 0, sizeof(s_addr));					// 어떤 port와 어떤 ip를 사용할 것인지 
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// ip 주소
	s_addr.sin_family = AF_INET;						// 네트워크 방식 (IPv4) IPv4 - 32비트, 8비트씩 4부분으로 10진수로 표시, IPv6 - 128비트, 16비트씩 8부분으로 16진수로 표시
	s_addr.sin_port = htons(PORT);						// port 번호, htons = 네트워크가 이해할 수 있는 것으로 변형	

	if (bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {	// bind 시작, 정보 값 (port가 사용 중이라면)
		printf("Can not Bind\n");
		return -1;
	}

	if (listen(s_socket, 5) == -1) {	// listen 소켓 커널에 개통 요청, 동접자 5명
		printf("listen Fail\n");
		return -1;
	}

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		thread_id = pthread_create(&pthread, NULL, do_service, (void *)&c_socket);		

		printf("Client is connected !\n");
	}


	close(s_socket);
	return 0;
}

void *do_service(void *val) {
	char rcvBuffer[100];
	int rcvLen, n;
	int c_socket = *((int *)val);
	while(1){
		char *token;
                char *str[3];
                int i = 0;

		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		rcvBuffer[rcvLen] = '\0';
		printf("[%s] received\n", rcvBuffer);
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
			break;
		else if(!strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")))
			strcpy(buffer, "안녕하세요. 만나서 반가워요.");
		else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
			strcpy(buffer, "내 이름은 김건우야.");
		else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
			strcpy(buffer, "나는 25살이야.");
		else if(!strncasecmp(rcvBuffer, "strlen ", 7))
			sprintf(buffer, "내 문자열의 길이는 %d입니다.", strlen(rcvBuffer)-7);
		else if(!strncasecmp(rcvBuffer, "strcmp ", 7)){
			token = strtok(rcvBuffer, " ");
			while(token != NULL){
				str[i++] = token;
				token = strtok(NULL, " ");
			}
			if(i<3)
				sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
			else if(!strcmp(str[1], str[2])) //같은 문자열이면,
				sprintf(buffer, "%s와 %s는 같은 문자열입니다.", str[1], str[2]);
			else
				sprintf(buffer, "%s와 %s는 다른 문자열입니다.", str[1], str[2]);
		} else if (!strncasecmp(rcvBuffer, "readfile ", 9)) {
			i = 0;
			token = strtok(rcvBuffer, " ");
			while(token != NULL){
				str[i++] = token;
				token = strtok(NULL, " ");
			}
			if(i<2)
				sprintf(buffer, "readfile 기능을 사용하기 위해서는 readfile <파일명> 형태로 입력하시오.");
			FILE *fp = fopen(str[1], "r");
			if(fp){
				char tempStr[100];
				memset(buffer, 0, 100);
				while(fgets(tempStr, 100, (FILE *)fp)){
					strcat(buffer, tempStr);
				}
				fclose(fp);
			}else{
				sprintf(buffer, "파일이 없습니다.");
			}
		}else if (!strncasecmp(rcvBuffer, "exec ", 5)) {
			char *command;
			token = strtok(rcvBuffer, " "); //exec
			command = strtok(NULL, "\0");
			printf("command: %s\n", command);
			int result = system(command);
			if(result)
				sprintf(buffer, "[%s] 명령어가 실패하였습니다.", command);
			else
				sprintf(buffer, "[%s] 명령어가 성공하였습니다.", command);
			
		}else
			strcpy(buffer, "무슨 말인지 모르겠습니다.");
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}	
	close(c_socket);
}

