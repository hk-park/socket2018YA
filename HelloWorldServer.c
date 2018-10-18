#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 9000
 
char buffer[BUFSIZ] = "Hi, I'm server\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[BUFSIZ], *ptr, *ptr2;
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
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
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
		
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			
			if(strncmp(rcvBuffer, "안녕?", strlen("안녕")) == 0){
				strcpy(buffer, "쪼롱이 : 안녕하세요. 만나서 반가워요(왈왈)");
			} else if(strncmp(rcvBuffer, "이름이 뭐야?", strlen("이름이 뭐야"))==0){
				strcpy(buffer, "쪼롱이 : 제 이름은  쪼롱이예요.");
			} else if(strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야"))==0){
				strcpy(buffer, "쪼롱이 : 저는 3살이예요.");
			} else{
				strcpy(buffer, "대답할 수 없어요ㅠㅠ");
			}

			if(strncmp(rcvBuffer, "strlen ", 7) == 0){
				n = strlen(rcvBuffer)-7;
				sprintf(buffer, "문자의 길이는 : %d 글자예요.", n);
			} else if(strncmp(rcvBuffer, "strcmp ", 7) == 0){
				ptr = strtok(rcvBuffer, " ");

				ptr = strtok(NULL, " ");
				ptr2 = strtok(NULL, " ");

				if(strcmp(ptr, ptr2) == 0){
				    	
					strcpy(buffer, "두 문자는 서로 같아요.");
				} else {
				
					strcpy(buffer, "두 문자는 서로 달라요.");
				}
			}

			write(c_socket, buffer, strlen(buffer));
		}

		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
