#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "Hi, I'm server\n";
#define NAMESIZE 2 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100];
	char *  fileName[NAMESIZE];
	FILE * fp;
	char readBuffer[100];
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			
			else if(strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요") == 0)) //if the result is true, return 0
				//compare 5 letters
				strcpy(buffer, "안녕하세요. 만나서 반가워요.");
			else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
				strcpy(buffer, "내 이름은 이원경이야");
			else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
                                strcpy(buffer, "나는 21살이야.");	
			else if(!strncmp(rcvBuffer, "strlen", strlen("strlen")))
				sprintf(buffer, "length of the string is %d", strlen(rcvBuffer)-7);
				//sprintf는 프린트에프랑 똑같은 기능을 하는데 다만 문자열을 버퍼에 넣은뒤 출력	
			else if(!strncmp(rcvBuffer, "strcmp", strlen("strcmp")))
			{
				char * token;
				char * str[3];
				int i = 0;
				token = strtok(rcvBuffer, " ");
				//token = strcmp
				while(token != NULL)
				{

					str[i++] = token;
					token = strtok(NULL,"  ");
					//for example, rcvBuffer = "strcmp test park"
					//str[0] = "strcmp"
					//str[1] = "test"
					//str[2] = "park"
				}
				if(i<3)
					sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
				else if(!strcmp(str[1], str[2])) //str[1] == str2[2]
					sprintf(buffer, "%s와 %s는 같은 문자열입니다. ",str[1], str[2]);				else 
					sprintf(buffer, "%s와 %s는 다른  문자열입니다. ",str[1], str[2]);
				}else 
					sprintf(buffer, "무슨 말을 하는지 모르겠습니다");

			n = strlen(buffer);
			write(c_socket, buffer, n);
		
			if(!strncmp(rcvBuffer, "readfile", strlen("readfile"))){
				int i = 0;
				char * token;
				token = strtok(rcvBuffer, " ");
				while(token != NULL)
				{
					fileName[i] = token;
					token = strtok(NULL, " ");

				}
				fp = fopen(fileName[1], "r");
				
				if(fp)
				{

				 while(fgets(buffer,100 , (FILE *)fp))
				 
				write(c_socket, buffer,100);

				}else{

				printf("cannot find file\n");
				}
			}
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}
