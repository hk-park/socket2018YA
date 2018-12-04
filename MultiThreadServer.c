#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 10000
#define BUFSIZE 10000

char Hbuffer[BUFSIZE] = "안녕하세요. 만나서 반가워요\n";
char Nbuffer[BUFSIZE] = "내 이름은 ㅁㅁㅁ이야.\n";
char Abuffer[BUFSIZE] = "나는 ㅁㅁ살이야.\n";
char nothing[BUFSIZE] = "XXXXXXXX \n";
 
main( )
{
	FILE *fp;
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n, i, x;
	int rcvLen;
	char rcvBuffer[BUFSIZE], *ptr,  *ptr1, *ptr2, buffer[BUFSIZE], fbuffer[BUFSIZE];
	pthread_t pthread;
	int thr_id;

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
		thr_id = pthread_create(&pthread, NULL, do_service, (void*)&c_socket);
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[rcvLen] = '\0';
			printf("[%s] received\n", rcvBuffer);
			buffer[0] = '\0';
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
<<<<<<< HEAD
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
>>>>>>> 6b3ed4b6c12d8e1a6b7d5357867170a3b961ce60
		}
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);
}

void *do_service(void *data){
	int n;
	char rcvBuffer[BUFSIZE];
	int c_socket = *((int*)data);

while(1){
	if(readSize = read(c_socket, 	
