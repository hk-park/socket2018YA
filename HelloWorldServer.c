#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
#define BUFSIZE 100 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100];
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
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else if(!strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요"))) //클라이언트에서 안녕하세요 보내면 서버에서 대답
				strcpy(buffer, "안녕하세요. 만나서 반가워요.");
			else if(!strncmp(rcvBuffer, "이름이 뭐야?", strlen("이름이 뭐야?")))
				strcpy(buffer, "내 이름은 아론이야.");
			else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer, "나는 27살이야.");
			else if(!strncmp(rcvBuffer, "strlen ", strlen("strlen"))) //문자열 길이 출력
				sprintf(buffer, "문자열의 길이는 %d입니다.", strlen(rcvBuffer)-7); //printf처럼 출력될 내용을 저장할 곳 지정
			else if(!strncmp(rcvBuffer, "strcmp", strlen("strcmp"))){
				char *token;
				char *str[3];
				int i = 0;
				token = strtok(rcvBuffer, " ");
				//token = strcmp
				while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
					//예를 들어, rcvBuffer = "strcmp test park"
					//str[0] = "strcmp"
					//str[1] = str1 . 예제로 생각하면 test가 됨.
					//str[2] = str2. 예제로 생각하면 park이 됨.	
				}
				if(i < 3)
					sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
				else if(!strcmp(str[1], str[2])) //str[1] == strp[2]
					sprintf(buffer, "%s와 %s는 같은 문자열입니다.", str[1], str[2]);
				else
                                        sprintf(buffer, "%s와 %s는 다른 문자열입니다.", str[1], str[2]);
			}else
				sprintf(buffer, "무슨 말인지 모르겠습니다.");
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
