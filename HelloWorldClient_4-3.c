#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define IPADDR "127.0.0.1"

char wrBuffer[100];

int main(){
	int c_socket;
	struct sockaddr_in c_addr;
	char rcvBuffer[100]; // 서버에서 보내준 문자열을 저장하기 위한 변수
	int n;
	c_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	if(connect(c_socket, (struct sockaddr *)&c_addr, sizeof(c_addr)) == -1){
		printf("[ERR] cannot connect\n");
		close(c_socket);
		return -1;
	}
	
	while(1){
		fgets(wrBuffer, sizeof(wrBuffer), stdin);
		wrBuffer[strlen(wrBuffer)-1] = '\0';
		write(c_socket, wrBuffer, strlen(wrBuffer));
		if(strncasecmp(wrBuffer, "quit", 4) == 0 || strncasecmp(wrBuffer, "kill server", strlen("kill server")) == 0)	 //대소문자 상관없이 비교
			break;
		n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		if(n < 0){
			printf("[ERR] Cannot read\n");
			return -1;
		}
		rcvBuffer[n] = '\0';
		printf("%s",rcvBuffer);
	}

	close(c_socket);
	return 0;
}
