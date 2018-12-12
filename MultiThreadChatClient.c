#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>
#define CHATDATA 1024
#define IPADDR "127.0.0.1"
#define PORT 9000


void *do_send_chat(void *);
void *do_receive_chat(void *);
pthread_t thread_1, thread_2;
char    escape[ ] = "exit";
char    nickname[20];


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
    int status;

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

    write(c_socket, nickname, strlen(nickname));

    pthread_create(&thread_1, NULL, do_send_chat, (void*)&c_socket);
    pthread_create(&thread_2, NULL, do_receive_chat, (void*)&c_socket);
    pthread_join(thread_1, (void**)&status);
    pthread_join(thread_2, (void**)&status);

    close(c_socket);
}

void * do_send_chat(void *arg)
{
    char chatData[CHATDATA];
    char buf[CHATDATA];
    int n;
    int c_socket = *((int *) arg);
    while(1) {
        memset(buf, 0, sizeof(buf));
        if((n = read(0, buf, sizeof(buf))) > 0 ) {
            sprintf(chatData, "[%s] %s", nickname, buf);
            write(c_socket, chatData, strlen(chatData));
            if(!strncmp(buf, escape, strlen(escape))) {
                pthread_kill(thread_2, SIGINT);
                break;
            }
        }
    }
}

void *do_receive_chat(void *arg)
{
    char    chatData[CHATDATA];
    int    n;
    int    c_socket = *((int *)arg);
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) {
            write(1, chatData, n);
        }
    }
}
