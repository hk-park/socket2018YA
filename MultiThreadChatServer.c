#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>
void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제
pthread_t thread;
pthread_mutex_t mutex;
#define MAX_CLIENT 10
#define CHATDATA 1024
#define IPADDR "127.0.0.1"
#define INVALID_SOCK -1
#define PORT 9000
void *do_send_chat(void *);
void *do_receive_chat(void *);
pthread_t thread_1, thread_2;
int    list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
char    escape[ ] = "exit";
char    nickname[20];
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
int main(int argc, char *argv[ ])
{
    int c_socket;
    struct sockaddr_in c_addr;
    int len;
    char chatData[CHATDATA];
    char buf[CHATDATA];
    int nfds;
    fd_set read_fds;
    int n;
    c_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&c_addr, 0, sizeof(c_addr));
    c_addr.sin_addr.s_addr = inet_addr(IPADDR);
    c_addr.sin_family = AF_INET;
    c_addr.sin_port = htons(PORT);
    printf("Input Nickname : ");
    scanf("%s", nickname);
    if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
        printf("Can not connect\n");
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Can not create mutex\n");
        return -1;
    }
    //pthread_create with do_send function
    //pthread_create with do_receive_chat function
    //pthread_join both threads
    close(c_socket);
}
void * do_send_chat(void *arg)
{
    char chatData[CHATDATA];
    char buf[CHATDATA];
    int n;
    int c_socket = *((int *) arg);        // client socket
    s_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
        printf("Can not Bind\n");
        return -1;
    }
    if(listen(s_socket, MAX_CLIENT) == -1) {
        printf("listen Fail\n");
        return -1;
    }
    for(i = 0; i < MAX_CLIENT; i++)
        list_c[i] = INVALID_SOCK;
    while(1) {
        memset(buf, 0, sizeof(buf));
        if((n = read(0, buf, sizeof(buf))) > 0 ) { //키보드에서 입력 받은 문자열을 buf에 저장. read()함수의 첫번째 인자는 file descriptor로써, 0은 stdin, 즉 키보드를 의미함.
            sprintf(chatData, "[%s] %s", nickname, buf);
            write(c_socket, chatData, strlen(chatData)); //서버로 채팅 메시지 전달
            if(!strncmp(buf, escape, strlen(escape))) { //'exit' 메세지를 입력하면,
                pthread_kill(thread_2, SIGINT); //do_receive_chat 스레드를 종료시킴
                break; //자신도 종료
            }
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            //pthread_create with do_chat function.
        }
    }
}
void *do_receive_chat(void *arg)
void *do_chat(void *arg)
{
    char    chatData[CHATDATA];
    int    n;
    int    c_socket = *((int *)arg);        // client socket
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {
            write(1, chatData, n); //chatData를 화면에 출력함 (1 = stdout (모니터))
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
            //write chatData to all clients
            //
            ///////////////////////////////
            if(strstr(chatData, escape) != NULL) {
                popClient(c_socket);
                break;
            }
        }
    }
}
int pushClient(int c_socket) {
    //ADD c_socket to list_c array.
    //
    ///////////////////////////////
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.
}
int popClient(int c_socket)
{
    close(c_socket);
    //REMOVE c_socket from list_c array.
    //
    ///////////////////////////////////
}
