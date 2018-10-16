#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000

char buffer[100] = "Hi, I'm server\n";
char rcvBuffer[100];

int main(){
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int len;
    int n,rcvLen;

    s_socket = socket(PF_INET, SOCK_STREAM, 0);

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP 할당
    s_addr.sin_family = AF_INET; //어떤 네트워크 프로토콜을 쓸지
    s_addr.sin_port = htons(PORT);

    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1){
			printf("Can not Bind\n");
 			return -1;
    }

    if(listen(s_socket, 5) == -1) {
			printf("listen Fail\n");
			return -1;
    }

    while(1){
			len = sizeof(c_addr);
			c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
			
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer)); //서버소켓은 클라이언트가 접속할때만 사용, read write 할때는 클라이언트 소켓을 사용
			rcvBuffer[rcvLen] = '\0';
			printf("%s",rcvBuffer);
			 
			n = strlen(buffer);
			write(c_socket, buffer, n);

			close(c_socket);
		}
	close(s_socket);
}
