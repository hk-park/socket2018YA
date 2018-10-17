#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define IPADDR "127.0.0.1"

int main(){
	int c_socket;
	struct sockaddr_in c_addr;
	int n;
	char rcvBuffer[100];
	char sendBuffer[100];
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
		fgets(sendBuffer, sizeof(sendBuffer), stdin);
		//sendBuffer[strlen(sendBuffer)-1] = '\0'; // 자기가 보내는 문자의 마지막을 \0 처리하는 것이지만 안하는게 좋음.
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
		printf("received Data[%d byte]: %s\n", n, rcvBuffer); 
	}	
	close(c_socket);
	return 0;

}
