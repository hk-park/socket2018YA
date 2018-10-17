#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h> 
#define PORT 9000

main()
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100];
	char sendBuffer[100];
	char cmp1[100];
	char cmp2[100];
	int cmpResult;
	char *cmpPtr;
	s_socket = socket(PF_INET, SOCK_STREAM, 0);

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
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while (1) {
			// 스트링을 클라이언트에서 받고, 받은 스트링에 대해 \n 문자를 Null로 바꿔줌.
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			if (rcvBuffer[rcvLen - 1] = '\n')
				rcvBuffer[rcvLen - 1] = '\0';
			printf("클라이언트에서 받은 데이터[%d byte]: %s\n", rcvLen, rcvBuffer);
			// 길이와 받은 스트링을 출력한다.

			// 서버 종료 기능
			if ((strncasecmp(rcvBuffer, "quit", 4) == 0) 
			|| (strncasecmp(rcvBuffer, "kill", 4) == 0))
				break;
			
			// 채팅용 기능
			else if ((strcasecmp(rcvBuffer, "hi") == 0) 
			|| (strcmp(rcvBuffer, "안녕") == 0))
				strcpy(sendBuffer, "Hello, Nice meet you.\n");
			else if ((strncasecmp(rcvBuffer, "How old are you", 15) == 0) 
			|| (strcmp(rcvBuffer, "몇살이니") == 0))
				strcpy(sendBuffer, "I am 3 days old.\n");
			else if ((strncasecmp(rcvBuffer, "What is your name", 17) == 0) 
			|| (strcmp(rcvBuffer, "이름이 뭐니") == 0))
				strcpy(sendBuffer, "I am HelloWorldServer.c!\n");

			// 함수 반환 기능
			else if (strncasecmp(rcvBuffer, "strlen", 6) == 0) { 
				// strlen 길이를 반환해준다
				sendBuffer[0] = '\0';
				strcpy(rcvBuffer, rcvBuffer + 7);
				sprintf(sendBuffer, "문자열 [%s]의 바이트는 %d\n", rcvBuffer, strlen(rcvBuffer));
			}
			else if (strncasecmp(rcvBuffer, "strcmp", 6) == 0) { 
				// strcmp 문자열 두개가 동일시 참, 아니면 거짓 반환
				cmpPtr = strtok(rcvBuffer, " ");
				cmpPtr = strtok(NULL, " ");
				strcpy(cmp1, cmpPtr);
				cmpPtr = strtok(NULL, " ");
				strcpy(cmp2, cmpPtr);
				if(strcmp(cmp1, cmp2) == 0)
					cmpResult = 0;
				else
					cmpResult = 1;
				
				sprintf(sendBuffer, "[%s]와 [%s]의 strcmp 결과값은 %d\n", cmp1, cmp2, cmpResult);
			}
			else
				sprintf(sendBuffer, "%s는 올바른 명령어가 아닙니다.\n", rcvBuffer);


			// 스트링을 클라이언트로 보냄
			n = strlen(sendBuffer);
			write(c_socket, sendBuffer, n);
			rcvBuffer[0] = '\0';
		}
		close(c_socket);
		if ((!strncasecmp(rcvBuffer, "kill", 4)) 
		|| (!strncasecmp(rcvBuffer, "quit", 4)))
			break;
	}
	close(s_socket);
}
