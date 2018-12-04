#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
#include<pthread.h>

#define PORT 9000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count=0;
char buffer[BUFSIZ] = "";

void *do_service(void *data);
 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	char buffer[BUFSIZ];

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
		printf("Client is connected\n");		
		
		pthread_mutex_lock(&mutex);
		count++;
		pthread_mutex_unlock(&mutex);
		printf("현재 %d개의 클라이언트가 접속하였습니다.\n", count);

		thr_id = pthread_create(&pthread, NULL, do_service, (void*)&c_socket);
	}	
}

void *do_service(void *data){
	int   n;
	int rcvLen;
	char rcvBuffer[BUFSIZ];

	char *token;
	char *str[3];
	int i = 0;

	FILE *fp;

	int c_socket = *((int*)data);

	while(1){
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		rcvBuffer[rcvLen] = '\0';
		printf("Received Data From Client: %s\n", rcvBuffer);
	
		if(strncasecmp(rcvBuffer, "quit", 4) == 0)	break;
		
		if(strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")) == 0){
			strcpy(buffer, "안녕하세요. 만나서 반가워요.");
		} else if(strncmp(rcvBuffer, "이름이 뭐야?", strlen("이름이 뭐야?"))==0){
			strcpy(buffer, "내 이름은 김하누리야");
		} else if(strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?"))==0){
			strcpy(buffer, "나는 23살이야");
		} else if(strncmp(rcvBuffer, "strlen ", 7) == 0){
			n = strlen(rcvBuffer)-7;
			sprintf(buffer, "%d", n);
		} else if(strncmp(rcvBuffer, "strcmp ", 7) == 0){
			i=0;
			token = strtok(rcvBuffer, " ");
				while(token != NULL){
				str[i++] = token;
				token = strtok(NULL, " ");
			}
			if(i<3){
				sprintf(buffer, "error");
			} else if(!strcmp(str[1], str[2])){
				sprintf(buffer, "%d", 0);
			} else {
				sprintf(buffer, "%d", -1);
				}
		} else if(strncmp(rcvBuffer, "readfile ", 9) == 0){
			i=0;
			token = strtok(rcvBuffer, " ");
				
			while(token != NULL){
				str[i++] = token;
				token = strtok(NULL, " ");
			}
			if(i<2){
				sprintf(buffer, "error");
			}

			fp = fopen(str[1], "r");
			if(fp){
				char temp[BUFSIZ];
				memset(buffer, 0, BUFSIZ);

				while(fgets(temp, BUFSIZ, (FILE*)fp)){
					strcat(buffer, temp);
				}
				fclose(fp);			
			} else {
				sprintf(buffer, "No file");
			}

		} else if(strncmp(rcvBuffer, "exec ", 5)==0){
			char *command;
			token = strtok(rcvBuffer, " ");
			command = strtok(NULL, "\0");
			printf("command: %s\n", command);
			int result = system(command);
			if(result)
				sprintf(buffer, "error");
			else
				sprintf(buffer, "success");
		} else {
			strcpy(buffer, "No data");
		}

		write(c_socket, buffer, strlen(buffer));
		memset(buffer, 0, sizeof(buffer));
	}

		pthread_mutex_lock(&mutex);
		count--;
		pthread_mutex_unlock(&mutex);
}

