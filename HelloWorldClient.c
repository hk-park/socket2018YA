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
	n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
	if(n < 0){
		printf("[ERR] Cannot read\n");
		return -1;
	}
	rcvBuffer[n] = '\0';
	printf("received Data: %s\n", rcvBuffer);
	// 3-2. 서버로부터 받은 문자열 길이 출력
	printf("received data length: %d\n", n);	
	close(c_socket);
	return 0;

}
