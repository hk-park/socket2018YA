#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

// 2-1. 서버 프로그램이 사용하는 포트
#define PORT 9000

// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
char buffer[100] = "Hi, I'm Server.\n";

int main()
{
	int c_socket, s_socket, rcvLen;		// 변수 선언
	struct sockaddr_in s_addr, c_addr;	// 클라이언트에서 보낸 데이터 받음
	char rcvBuffer[100];				// 소켓 통신 정보 (구조체)	

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

	while (1) {
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &sizeof(c_addr));

		printf("Client is connected\n");
		while (1) {
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if (strncasecmp(rcvBuffer, "quit", strlen("quit")) == 0 || strncasecmp(rcvBuffer, "kill server", strlen("kill server")) == 0)
				break;
			else if (!strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")))
				strcpy(buffer, "안녕하세요. 만나서 반가워요.");
			else if (!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
				strcpy(buffer, "내 이름은 김건우야.");
			else if (!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer, "나는 25살이야.");
			else if (!strncasecmp(rcvBuffer, "strlen ", strlen("strlen ")))
				sprintf(buffer, "문자열 길이 : %d", strlen(rcvBuffer) - strlen("strlen "));
			else if (!strncasecmp(rcvBuffer, "strcmp ", strlen("strcmp "))) {
				int i = 0;
				char *split[3];
				char *ptr = strtok(rcvBuffer, " ");
				while (ptr != NULL) {
					split[i++] = ptr;
					ptr = strtok(NULL, " ");
				}
				if (i < 3)
					sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
				else if (!strcmp(split[1], split[2]))
					sprintf(buffer, "같은 문자 : 0");
				else
					sprintf(buffer, "다른 문자 : -1");
			}
			else
				strcpy(buffer, "Match Type : X");

			write(c_socket, buffer, strlen(buffer));
		}
		close(c_socket);
		if (!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}
	close(s_socket);
	return 0;
}
