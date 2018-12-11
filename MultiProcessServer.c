#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
#define BUFSIZE 10000 
#define MAXCLIENT 3
#define CHATDATA 1024
#define MAX_NAME 20
#define INVALID_SOCK -1

// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[BUFSIZE] = "hello, world\n";
struct user {
        int user_socket;
        char user_name[MAX_NAME];
}; // 사용자 닉네임

struct user	list_user[MAXCLIENT];
// 클라이언트 구조체 배열을 만든다.

char buffer[BUFSIZE] = "Hi, I'm server\n";
int numClient = 0;
void* do_chat(void *);
int pushClient(int c_socket);
int popClient(int s);
void sig_handler(int signo);

pthread_t thread;
pthread_mutex_t mutex;


main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int pid;
	int   len;
	signal(SIGCHLD, sig_handler);
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
 
	if(listen(s_socket, MAXCLIENT) == -1) {
		printf("서버가 포화 상태입니다.");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		numClient++;
		printf("현재 %d개의 클라이언트가 접속하였습니다.\n", numClient);
		pid = fork();
		if(pid > 0){ //부모 프로세스
			close(c_socket);
		}else if(pid == 0){ //자식 프로세스
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}else{
			printf("[ERROR] fork failed\n");
			exit(0);
		}
	}	
	close(s_socket);
}

void do_service(int c_socket){
	int   n;
	int rcvLen;
	char rcvBuffer[BUFSIZE];
	while(1){
		char *token;
		char *str[3];
		int i = 0;
		rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		rcvBuffer[rcvLen] = '\0';
		printf("[%s] received\n", rcvBuffer);
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
			break;
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
		n = strlen(buffer);
		write(c_socket, buffer, n);
	}
}
void sig_handler(int signo){
	int pid;
	int status;
	pid = wait(&status);//자식 프로세스가 종료될 때까지 기다려주는 함수
	printf("pid[%d] process terminated.status = %d\n", pid, status);
	numClient--;
	printf("1개의 클라이언트가 접속종료되어 %d개의 클라이언트가 접속되어 있습니다.\n", numClient);
}
void * do_chat(void *arg) {
        int c_socket = (int) arg;
        char chatData[CHATDATA];
        char tempData[CHATDATA];
        int     i, n;

        char *whisper_user;

        while(1) {
                memset(chatData, 0, sizeof(chatData));

                // 값이 들어오면 값을 카피한 후 귓속말 대상을 토크 나이저로 분리한다.
                if ((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {
			strcpy(tempData, chatData);
			whisper_user = strtok(tempData, "/");
			whisper_user = strtok(NULL, "/");
                // 귓속말 대상의 값이 있으면 해당 클라이언트에게만 메시지를 보낸다.
			printf("whisper_user is %s\n", whisper_user);

			for (i = 0; i < MAXCLIENT; i++) {
	                        if (list_user[i].user_socket != INVALID_SOCK
        	                &whisper_user != NULL && !strcmp(list_user[i].user_name, whisper_user)) {
                	        printf("whisper, from %s to %s.\n",
                        	list_user[i].user_name, whisper_user);
	                        write(list_user[i].user_socket, chatData, n);
        	        	} else if (list_user[i].user_socket != INVALID_SOCK && whisper_user == NULL) {
                	        write(list_user[i].user_socket, chatData, n);
                		}
        		}
		        if(strstr(chatData, "exit") != NULL) {
                		popClient(c_socket);
		                break;
			}

		}
	}
}

int pushClient(int c_socket) {
        int     i, n;
        char    user_name[MAX_NAME];
        memset(user_name, 0, sizeof(user_name));
	// 클라이언트에서 소켓 연결과 동시에 닉네임 값을 보내주므로, 해당 값을 받아서 저장한다.
		for (i = 0; i < MAXCLIENT; i++) {
			pthread_mutex_lock(&mutex);
                        if(list_user[i].user_socket == INVALID_SOCK) {
				list_user[i].user_socket = c_socket;
                                if ((n = read(c_socket, user_name, sizeof(user_name))) > 0) {
					strcpy(list_user[i].user_name, user_name);
				}
			printf("Connected socket number is %d and name is %s\n",list_user[i].user_socket, list_user[i].user_name);
                        pthread_mutex_unlock(&mutex);
			return i;
			}
		pthread_mutex_unlock(&mutex);
		}
	
	if (i == MAXCLIENT)
		return -1;
}

int popClient(int s) {
    int i;
    close(s);

    for (i = 0; i < MAXCLIENT; i++) {
        pthread_mutex_lock(&mutex);
        if ( s == list_user[i].user_socket ) {
                list_user[i].user_socket = INVALID_SOCK;
                pthread_mutex_unlock(&mutex);
                break;
        }
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

