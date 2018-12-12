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
#define USERNAME 20

struct user{
	char name[USERNAME];
	int sock;
};

void *do_chat(void *); 
int pushClient(int); 
int popClient(int); 

struct user list_c[MAX_CLIENT];
char    greeting[ ] = "++WELCOME++\n";
char    CODE200[ ] = "[ERORR]\n";

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
	
	int i, n;
	char *token, *user, *text;
	char chatData[CHATDATA], send[CHATDATA], chk[USERNAME];
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			token = strtok(chatData, " ");
			token = strtok(NULL, " ");
			
			if(strncmp(token, "/w", 2)==0){ 
			
				user = strtok(NULL, " ");
				text= strtok(NULL, "\0");
				
				
				for(i=0; i<MAX_CLIENT; i++){
					if(list_c[i].sock == c_socket){
						strcpy(chk, list_c[i].name);
						break;
					}
				}

				
				for(i=0; i<MAX_CLIENT; i++){
					if(strncmp(list_c[i].name, user, strlen(user))==0){
						sprintf(send, "[%s] %s", chk, text);
						write(list_c[i].sock, send, n);
					}
				}
			}
		
			else if(!strncasecmp(chatData,"exit",4)) {
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

int pushClient(int c_socket) {
	int i, n;
	char user_name[USERNAME];

	n = read(c_socket, user_name, sizeof(user_name));
	user_name[n] = "\0";

	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock==INVALID_SOCK){
			list_c[i].sock = c_socket;
			strcpy(list_c[i].name, user_name);
			printf("[%s]in.\n",list_c[i].name);
			return i;
		}
	}
	
	if(i==MAX_CLIENT) return -1;
}

int popClient(int c_socket){
	int i, j;
	

	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock == c_socket){
			printf("[%s]out.\n",list_c[i].name);
			for(j=i; j<MAX_CLIENT; j++){
				list_c[i].sock = list_c[j+1].sock;
				if(list_c[j].sock == INVALID_SOCK) break;
			}
		}
	}
	close(c_socket);
	return j;
}
