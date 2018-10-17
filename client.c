nclude <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 10000
#define IPADDR "127.0.0.1"

int main(void){
	int c_socket;
	struct sockaddr_in c_addr;
	char rcvbuf[100] = "\0";
	char buffer[100];
	int n;

	c_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	if(connect(c_socket, (struct sockaddr *)&c_addr, sizeof(c_addr))==-1){
		printf("[ERROR] Cannot connect\n\n");
		close(c_socket);
		return -1;
	}
	n = read(c_socket, rcvbuf, sizeof(rcvbuf));
	printf("%s", rcvbuf);

	while(1){
		printf("INSERT ]]] ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strlen(buffer)-1]='\0';
		write(c_socket, buffer, strlen(buffer));
		if(strncmp(buffer, "quit", 4)==0 || strncmp(buffer, "kill server", 11)==0)
			break;

		n = read(c_socket, rcvbuf, sizeof(rcvbuf));
		if(n<0){
			printf("[ERROR] CAN NOT READ.\n");
			return -1;
		}
		rcvbuf[n] = '\0';
		printf("SERVER : %s\n", rcvbuf);
	}
	close(c_socket);
	printf("======= EXIT =======\n");

	return 0;
}
