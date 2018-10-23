#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 10000
 
char buffer[100] = "Hi. I`m server.\n";
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	char rcvBuffer[100];
	int rcvLen;
	FILE *fp;


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
		while(1){
			printf("client is connected\n");
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			if(strncmp(rcvBuffer,"readfile",8)==0) {
				char *token;
				char *str[2];
				int i=0;

				token=strtok(rcvBuffer," ");			
				while(token != NULL) {
					str[i++]=token;
					token=strtok(NULL, " ");							
				}
				fp=fopen(str[1],"r");
				
				printf("%s",str[1]);	
				
				fclose(fp);
			}
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] is received\n", rcvBuffer);
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}
	}	
	close(s_socket);
}
