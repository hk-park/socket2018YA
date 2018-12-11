#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제
pthread_t thread;
pthread_mutex_t mutex;
#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
struct user{
	int c_socket;
	char c_name[20];
};//접속한 클라이언트 관리구조체

struct   user  list_c[MAX_CLIENT]; 
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Can not create mutex\n");
        return -1;
    }
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
        list_c[i].c_socket = INVALID_SOCK;
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            pthead_create(&thread, NULL, do_chat, (void *) &c_socket);
        }
    }
}
void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
    char *ptr, *ptrSender, *ptrReceiver;
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
            ptr = strstr(chatData, "/w"); //w가 위치한 포인터 저장
            if(strstr(chatData, escape) != NULL) {
		ptr = strtok(chatData, " ");//토큰내용 : "[보낸사람 닉네임]
		ptrSender = ptr;
		ptr = strtok(NULL, " ");//토큰내용 : "/W"
		ptr = strtok(NULL, " ");//토큰내용 : "받는사람 닉네임"
		ptrReceiver = ptr;
		ptr = strtok(NULL, "\n");//토큰내용 : "메세지"
		for(i=0; i<MAX_CLIENT; i++){
			if(!strcmp(list_c[i].c_name, ptrReceiver)){//받는사람 닉네임 토큰과 구조체의 유저 닉네임이 일치할 경우
			sprintf(chatData, "%s(귓) %s\n\0", ptrSender, ptr);
			write(list_c[i].c_socket, chatData, strlen(chatData));//귓속말 보내기
			continue;
	        	}
	         }//for문 닫힘
	      }else{
			for(i=0; i<MAX_CLIENT; i++){
				if(list_c[i].c_socket != INVALID_SOCK && list_c[i].c_socket!=c_socket)//메세지 보낸 사람에게는 메세지를
					write(list_c[i].c_socket,chatData,n);
			}
			if(strstr(chatData, escape) != NULL){
		                popClient(c_socket);
		                break;
            }
        }//else 닫힘
    }//if닫힘
  }//while닫힘
}
int pushClient(int c_socket) {
    int i;
    char nameData[CHATDATA];
    for(i=0; i<MAX_CLIENT; i++){
	if(list_c[i].c_socket == INVALID_SOCK){
		pthread_mutex_lock(&mutex);
		list_c[i].c_socket=c_socket;
		pthread_mutex_unlock(&mutex);
		memset(nameData, 0, sizeof(nameData));
		if(read(list_c[i].c_socket,nameData, sizeof(nameData)) > 0){ //클라이언트로부터 닉네임 입력받음
		pthread_mutex_lock(&mutex);
		strcpy(list_c[i].c_name, nameData); //입력받은 닉네임을 구조체의 유저 닉네임에 저장
		pthread_mutex_unlock(&mutex);
	}
   	return i;
      }
   }
  if(i==MAX_CLIENT)
	 return 0;
}
int popClient(int c_socket)
{
    int i;	
    close(c_socket);
    for(i=0; i<MAX_CLIENT; i++){
	if(c_socket==list_c[i].c_socket){
		pthread_mutex_lock(&mutex);
		list_c[i].c_socket = INVALID_SOCK;
		pthread_mutex_unlock(&mutex);
		break;
	}	
    }
  return 0;
}
