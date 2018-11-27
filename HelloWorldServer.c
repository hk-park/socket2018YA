#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
<<<<<<< HEAD
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
#define BUFSIZE 10000 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[BUFSIZE] = "hello, world\n";
char buffer[BUFSIZE] = "Hi, I'm server\n";
=======
#define PORT 10000
<<<<<<< HEAD
#define BUFSIZE 10000
char Hbuffer[BUFSIZE] = "안녕하세요. 만나서 반가워요\n";
char Nbuffer[BUFSIZE] = "내 이름은 ㅁㅁㅁ이야.\n";
char Abuffer[BUFSIZE] = "나는 ㅁㅁ살이야.\n";
char nothing[BUFSIZE] = "XXXXXXXX \n";
=======
 
char buffer[100] = "Hi. I`m server.\n";
>>>>>>> aff4d2d589d280b5bd74c2c684decb2df6755550
>>>>>>> fd0e397dc3c28da15ff9db1ea4e5202729833fef
 
main( )
{
	FILE *fp;
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
<<<<<<< HEAD
	int   n, i, x;
	int rcvLen;
<<<<<<< HEAD
	char rcvBuffer[BUFSIZE];
=======
	char rcvBuffer[BUFSIZE], *ptr,  *ptr1, *ptr2, buffer[BUFSIZE], fbuffer[BUFSIZE];
=======
	int   n;
	char rcvBuffer[100];
	int rcvLen;
	FILE *fp;


>>>>>>> aff4d2d589d280b5bd74c2c684decb2df6755550
>>>>>>> fd0e397dc3c28da15ff9db1ea4e5202729833fef
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
<<<<<<< HEAD
		printf("Client is connected\n");
=======
>>>>>>> aff4d2d589d280b5bd74c2c684decb2df6755550
		while(1){
<<<<<<< HEAD
			char *token;
			char *str[3];
			int i = 0;
=======
			printf("client is connected\n");
>>>>>>> fd0e397dc3c28da15ff9db1ea4e5202729833fef
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
<<<<<<< HEAD
			printf("[%s] received\n", rcvBuffer);
			buffer[0] = '\0';
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
<<<<<<< HEAD
			else if(!strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")))
				strcpy(buffer, "안녕하세요. 만나서 반가워요.");
			else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
				strcpy(buffer, "내 이름은 박홍규야.");
			else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer, "나는 30살이야.");
			else if(!strncasecmp(rcvBuffer, "strlen ", 7))
				sprintf(buffer, "내 문자열의 길이는 %d입니다.", strlen(rcvBuffer)-7);
			else if(!strncasecmp(rcvBuffer, "strcmp ", 7)){
				i = 0;
				token = strtok(rcvBuffer, " ");
				while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
				}
				if(i<3)
					sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
				else if(!strcmp(str[1], str[2])) //같은 문자열이면,
					sprintf(buffer, "%s와 %s는 같은 문자열입니다.", str[1], str[2]);
				else
					sprintf(buffer, "%s와 %s는 다른 문자열입니다.", str[1], str[2]);
			}else if (!strncasecmp(rcvBuffer, "readfile ", 9)) {
				i=0;
				token = strtok(rcvBuffer, " ");
				while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
				}
				//str[0] = readfile
				//str[1] = filename
				if(i<2)
					sprintf(buffer, "readfile 기능을 사용하기 위해서는 readfile <파일명> 형태로 입력하시오.");
				FILE *fp = fopen(str[1], "r");
				if(fp){
					char tempStr[BUFSIZE];
					memset(buffer, 0, BUFSIZE);
					while(fgets(tempStr, BUFSIZE, (FILE *)fp)){
						strcat(buffer, tempStr);
					}
					fclose(fp);
				}else{
					sprintf(buffer, "파일이 없습니다.");
				}
			}else if (!strncasecmp(rcvBuffer, "exec ", 5)) {
				char *command;
				token = strtok(rcvBuffer, " "); //exec
				command = strtok(NULL, "\0");
				printf("command: %s\n", command);
				int result = system(command);
				if(result)
					sprintf(buffer, "[%s] 명령어가 실패하였습니다.", command);
				else
					sprintf(buffer, "[%s] 명령어가 성공하였습니다.", command);		
			}else
				 strcpy(buffer, "무슨 말인지 모르겠습니다.");
=======
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
=======
			printf("[%s] is received\n", rcvBuffer);
>>>>>>> fd0e397dc3c28da15ff9db1ea4e5202729833fef
			n = strlen(buffer);
			write(c_socket, buffer, n);
>>>>>>> aff4d2d589d280b5bd74c2c684decb2df6755550
		}
	}	
	close(s_socket);
}
