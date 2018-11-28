#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
<<<<<<< HEAD

// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
#define BUFSIZE 100

char buffer[BUFSIZE] = "Hi, I'm server\n";
char rcvBuffer[BUFSIZE];
=======
#define PORT 10000
#define BUFSIZE 10000
char Hbuffer[BUFSIZE] = "안녕하세요. 만나서 반가워요\n";
char Nbuffer[BUFSIZE] = "내 이름은 ㅁㅁㅁ이야.\n";
char Abuffer[BUFSIZE] = "나는 ㅁㅁ살이야.\n";
char nothing[BUFSIZE] = "XXXXXXXX \n";
 
>>>>>>> upstream/master
main( )
{
	FILE *fp;
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
<<<<<<< HEAD
	int   n;
	int readSize;
	char *read_text;
	char tempBuf[BUFSIZE];
=======
	int   n, i, x;
	int rcvLen;
	char rcvBuffer[BUFSIZE], *ptr,  *ptr1, *ptr2, buffer[BUFSIZE], fbuffer[BUFSIZE];
>>>>>>> upstream/master
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
<<<<<<< HEAD
			memset(rcvBuffer, 0, BUFSIZE);
			if((readSize = read(c_socket, rcvBuffer, sizeof(rcvBuffer)))<0){
				return -1;
			}
			printf("Received Data From Client: %s\n", rcvBuffer);
			if(strcasecmp(rcvBuffer, "quit")== 0 || strcasecmp(rcvBuffer, "kill server")==0){
				break;
			}else if(strcasecmp(rcvBuffer, "kill server") == 0){
				break;
			}else if(strncasecmp(rcvBuffer, "readfile",8)==0){
				read_text = strtok(rcvBuffer, " ");
				read_text = strtok(NULL, " ");
				FILE *fp = fopen(read_text, "r");
				char tempStr[BUFSIZE];
				memset(buffer, 0, BUFSIZE);
				while(fgets(tempStr, BUFSIZE, (FILE *)fp)){
					strcat(buffer, tempStr);
				}
				fclose(fp);
			}

			printf("[%s] received\n", rcvBuffer);
			n = strlen(buffer);
			write(c_socket, buffer, n);
=======
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			buffer[0] = '\0';
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			if(!strncasecmp(rcvBuffer, "안녕하세요.", 6)){
				n = strlen(Hbuffer);
				write(c_socket, Hbuffer, n);
			}
			else if(!strncasecmp(rcvBuffer, "이름이 뭐야?", 7)){
				n = strlen(Nbuffer);
				write(c_socket, Nbuffer, n);
			}
			else if(!strncasecmp(rcvBuffer, "몇 살이야?", 6)){
				n = strlen(Abuffer);
				write(c_socket, Abuffer, n);
			}
			else if(!strncasecmp(rcvBuffer, "strlen", 6)){
				x = strlen(rcvBuffer)-7;			
				sprintf(buffer,"길이 : %d",x);
				write(c_socket, buffer, strlen(buffer));
			}
			else if(!strncasecmp(rcvBuffer, "strcmp", 6)){
				ptr = strtok(rcvBuffer, " ");
				ptr = strtok(NULL, " ");
				ptr1 = ptr;
				ptr = strtok(NULL, " ");
				ptr2 = ptr;
				x = strcmp(ptr1, ptr2);
				sprintf(buffer,"결과 : %d",x);
				write(c_socket, buffer, strlen(buffer));
			}
			else if(!strncasecmp(rcvBuffer, "readfile", 8)){
				ptr = strtok(rcvBuffer, " ");
				ptr = strtok(NULL, " ");
				ptr1 = ptr;
				fp = fopen(ptr1, "r");
				if(fp){
					while(fgets(fbuffer, BUFSIZE, (FILE *)fp))
							strcat(buffer, fbuffer);
				}
				else{
					strcpy(buffer, "존재하지 않는 파일");
				}
				fclose(fp);	
				write(c_socket, buffer, strlen(buffer));			
			}
			else if(!strncasecmp(rcvBuffer, "exec", 4)){
				ptr = strtok(rcvBuffer, " ");
				ptr = strtok(NULL, "NULL");
				ptr1 = ptr;			
				int ret = system(ptr1);
				if(!ret)
					sprintf(buffer, "<%s> command Success!!\n", ptr);
				else
					sprintf(buffer, "<%s> command Failed!!\n", ptr);
				write(c_socket, buffer, strlen(buffer));
			}
			else{
				n = strlen(nothing);
				write(c_socket, nothing, n);
			}
>>>>>>> upstream/master
		}
		close(c_socket);
		
	}	
	close(s_socket);
}
