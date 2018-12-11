#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/wait.h> //I don't know if needed
#include <pthread.h>
#include <signal.h>
#define PORT 9000
#define CHATDATA 1024
#define IPADDR "127.0.0.1"

void *do_send_chat(void *);
void *do_receive_chat(void *);
void sig_handler(int);
pthread_t thread_1, thread_2;
char    escape[ ] = "exit";
char    nickname[20];
int c_socket;

int main()
{
    struct sockaddr_in c_addr;
    int len;
    char chatData[CHATDATA];
    char buf[CHATDATA];
    int nfds;
    fd_set read_fds;
    int n;
	signal(SIGINT, sig_handler); //ctrl+c로 클라이언트 종료시 설정.
    c_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&c_addr, 0, sizeof(c_addr));
    c_addr.sin_addr.s_addr = inet_addr(IPADDR);
    c_addr.sin_family = AF_INET;
    c_addr.sin_port = htons(PORT);
    printf("Input Nickname : ");
    scanf("%s", nickname);
    if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
        printf("Can not connect\n");
        return -1;
    }
    //pthread_create with do_send function
	thread_1 = pthread_create(&thread_1, NULL, do_send_chat, (void*)&c_socket);
    //pthread_create with do_receive_chat function
	thread_2 = pthread_create(&thread_1, NULL, do_receive_chat, (void*)&c_socket);
    //pthread_join both threads
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
    close(c_socket);
}

void * do_send_chat(void *arg)
{
    char chatData[CHATDATA];
    char buf[CHATDATA];
    int n;
    int c_socket = *((int *) arg);        // client socket
    while(1) {
		//printf(" [client]# ");
        memset(buf, 0, sizeof(buf));
        if((n = read(0, buf, sizeof(buf))) > 0 ) { //키보드에서 입력 받은 문자열을 buf에 저장. read()함수의 첫번째 인자는 file descriptor로써, 0은 stdin, 즉 키보드를 의미함.
            sprintf(chatData, "[%s] %s", nickname, buf);
            write(c_socket, chatData, strlen(chatData)); //서버로 채팅 메시지 전달
            if(!strncmp(buf, escape, strlen(escape))) { //'exit' 메세지를 입력하면,
                pthread_kill(thread_2, SIGINT); //do_receive_chat 스레드를 종료시킴
                break; //자신도 종료
            }
        }
    }
}
void *do_receive_chat(void *arg)
{
    char    chatData[CHATDATA];
    int    n;
    int    c_socket = *((int *)arg);        // client socket
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {
            write(1, chatData, n); //chatData를 화면에 출력함 (1 = stdout (모니터))
        }
    }
}

void sig_handler(int signo){
	// ctrl + c로 클라이언트가 종료되면 서버에 접속종료 명령을 보내고, 클라이언트를 종료한다.
	printf(" <Ctrl + c> 입력됨. 클라이언트 종료 작업 수행.\n");
	write(c_socket, "quit\n", strlen("quit\n")); // 서버로 문자열을 보냄
	close(c_socket);
	exit(0);
}