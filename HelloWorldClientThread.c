#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>

#define PORT 9000
#define IPADDR "127.0.0.1"
int c_socket;
void sig_handler(int);

int main(){
	struct sockaddr_in c_addr;
	int n;
	char rcvBuffer[100];
	char sendBuffer[100];
	signal(SIGINT, sig_handler); //ctrl+c로 클라이언트 종료시 설정.
	//signal(SIGSTOP, sig_handler);
	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	
	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1){
		printf("[ERR] Cannot connect\n");
		close(c_socket);
		return -1;
	}
	while(1){
		printf(" [client]# ", n, rcvBuffer);
		fgets(sendBuffer, sizeof(sendBuffer), stdin);
		//sendBuffer[strlen(sendBuffer)-1] = '\0'; // 자기가 보내는 문자의 마지막을 \0 처리하는 것이지만 안하는게 좋음. 서버쪽에서 처리하게 할 것.
		write(c_socket, sendBuffer, strlen(sendBuffer)); // 서버로 문자열을 보냄
		if(strncasecmp(sendBuffer, "quit", 4) == 0 
			|| strncasecmp(sendBuffer, "kill", 4) == 0)
			break;
		n = read(c_socket, rcvBuffer, sizeof(rcvBuffer)); // 서버에서 문자열을 받음
		if(n < 0){
			printf("[ERR] Cannot read\n");
			return -1;
		}
		if(rcvBuffer[n-1] = '\n') //받은 문자열에 대해 개행문자 처리
			rcvBuffer[n-1] = '\0';
		printf("서버에서 받음[%d byte]: %s\n", n, rcvBuffer); 
	}	
	close(c_socket);
	return 0;

}


void sig_handler(int signo){
	// ctrl + c로 클라이언트가 종료되면 서버에 접속종료 명령을 보내고, 클라이언트를 종료한다.
	printf(" <Ctrl + c> 입력됨. 클라이언트 종료 작업 수행.\n");
	write(c_socket, "quit\n", strlen("quit\n")); // 서버로 문자열을 보냄
	close(c_socket);
	exit(0);
}
