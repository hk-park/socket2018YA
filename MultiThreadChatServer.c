#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
#define USERNAME 300

struct user{
	char name[USERNAME];
	int sock;
};

void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제

struct user list_c[MAX_CLIENT];
char    greeting[ ] = "WELLCOM! FREAND!! [ /w : 귓속말]\n";
char    CODE200[ ] = "[ERORR] : CAN NOT CONNECT MORE CLIENT.\n";

pthread_t thread;
pthread_mutex_t mutex;

int main(int argc, char *argv[]){
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int i, j, n;
	int res;

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
		list_c[i].sock = INVALID_SOCK;

	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		res = pushClient(c_socket);
		
		if(res < 0) {
			write(c_socket, CODE200, strlen(CODE200));
			close(c_socket);
		}
		else {
			write(c_socket, greeting, strlen(greeting));
			pthread_mutex_lock(&mutex);
			pthread_create(&thread, NULL, do_chat, (void*)&c_socket);
			pthread_mutex_unlock(&mutex);
		}
	}
}



void *do_chat(void *arg){
	int c_socket = *((int *)arg);
	char chatData[CHATDATA], tempData[CHATDATA], userChk[USERNAME];
	int i, n;
	char *chk, *uname, *msg;

	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			chk = strtok(chatData, " ");
			chk = strtok(NULL, " ");
			
			if(strncmp(chk, "/w", 2)==0){ 
			// '/w'을 타이핑 시, 귓속말 기능 수행
				uname = strtok(NULL, " ");//받는 유저 닉네임
				msg = strtok(NULL, "\0");//메세지
				
				//SEND USER
				for(i=0; i<MAX_CLIENT; i++){
					if(list_c[i].sock == c_socket){
						strcpy(userChk, list_c[i].name);//말하는 유저 이름 복사
						break;
					}
				}

				//RECIEVE USER
				for(i=0; i<MAX_CLIENT; i++){
					if(strncmp(list_c[i].name, uname, strlen(uname))==0){
						sprintf(tempData, "[%s] %s", userChk, msg);//받은 메세지 출력
						write(list_c[i].sock, tempData, n);
					}
				}
			}
		
			else if(!strncasecmp(chatData,"exit ",5)) {
				popClient(c_socket);
				break;
			}
			else {
			     for(i=0; i<MAX_CLIENT; i++){
				 if(list_c[i].sock==INVALID_SOCK)	break;
				 write(list_c[i].sock, chatData, n);
				}
			}
		}
	}
}



int pushClient(int c_socket) { //리스트에 c소켓 추가
	int i, n;
	char user_name[USERNAME];

	n = read(c_socket, user_name, sizeof(user_name));
	user_name[n] = "\0";

	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock==INVALID_SOCK){
			list_c[i].sock = c_socket;
			strcpy(list_c[i].name, user_name);
			printf("[%d] %s 님이 대화방에 방문 하셨습니다.\n", i+1, list_c[i].name);
			return i;
		}
	}
	
	if(i==MAX_CLIENT) return -1;
}

int popClient(int c_socket){
	int i, j;
	close(c_socket);

	// 리스트 배열로부터 c소켓 제거
	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock == c_socket){
			printf("[%d] %s 님이 대화방에서 나갔습니다 ! \n",i+1, list_c[i].name);
			for(j=i; j<MAX_CLIENT; j++){
				list_c[i].sock = list_c[j+1].sock;
				if(list_c[j].sock == INVALID_SOCK) break;
			}
		}
	}
	return j;
}

