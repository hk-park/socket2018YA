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

char	escape[ ] = "exit";
char	nickname[20];

int main(int argc, char *argv[ ])
{
	int c_socket;
	struct sockaddr_in c_addr;
	int len;
	int status;
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int nfds;
	fd_set read_fds;
	int n;
	char *token =NULL;
	char str[3][300];
	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	printf("Input Nickname : ");
	scanf("%s", nickname);
	getchar();
	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
		printf("Can not connect\n");
		return -1;
	}
	//pthread_create with do_send function
	//pthread_create with do_receive_chat function
	//pthread_join both threads
	pthread_create(&thread_1,NULL,do_send_chat,(void *) &c_socket);
        pthread_create(&thread_2,NULL,do_receive_chat, (void *) &c_socket);
        pthread_join(thread_1,(void **) &status);
        pthread_join(thread_2, (void **) &status);
	close(c_socket);
}
void * do_send_chat(void *arg)
{
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int n;
	char *token;
	char str[3][1024];
	int i=0;
	char imsi[CHATDATA];
	int c_socket = *((int *) arg);		// client socket
	while(1) {
		memset(buf, 0, sizeof(buf));
		if((n = read(0, buf, sizeof(buf))) > 0 ) {
		//      if(strncmp(buf,"wc",2)==0){
               	//      sprintf(chatData,"%s",buf);
                //      write(c_socket,chatData,strlen(chatData));
                //      continue;

                //      }
                //      else{   
                        sprintf(chatData, "[%s] %s", nickname, buf);
                        write(c_socket, chatData, strlen(chatData));
                //      }
                        if(!strncmp(buf, escape, strlen(escape))) {
                                pthread_kill(thread_2, SIGINT);
       break;
                        }
                }
        }
}

void *do_receive_chat(void *arg)
{
	char	chatData[CHATDATA];
	int	n;
	int	c_socket = *((int *)arg);		// client socket
	int i=0;
	 char *token=NULL;
	 char str[4][200];
    	 char imsi[CHATDATA];
    	 char test[CHATDATA];
    	 while(1) {
        	memset(chatData, 0, sizeof(chatData));
        	memset(str[0],0,sizeof(str[0]));
        	memset(str[1],0,sizeof(str[1]));
        	memset(str[2],0,sizeof(str[2]));
        	memset(str,0,sizeof(str));
        	memset(imsi,0,sizeof(imsi));
        	memset(test,0,sizeof(test));
        	token=NULL;
        	if((n = read(c_socket, chatData, sizeof(chatData))) > 0 )
       		 {
            		strcpy(imsi,chatData);
            		if(strstr(imsi,"/w")!=NULL){
                	token=strtok(imsi," ");
                	strcpy(str[0],token);
                	token=strtok(NULL," ");
                	strcpy(str[1],token);
                	token=strtok(NULL," ");
                	strcpy(str[2],token);
                	token=strtok(NULL," ");
                	strcpy(str[3],token);
                	if(strcasecmp(str[2],nickname)==0){
                    		strcat(test,str[0]);
                   		strcat(test," ");
                    		strcat(test,str[3]);
                    		write(1,test,n);
                    		memset(chatData,0,sizeof(chatData));
                    		memset(imsi,0,sizeof(imsi));
                    		continue;
               		 }
               		 else continue;
            	}
            	else if(strstr(imsi,"/w")==NULL) write(1, chatData, n);
	      }
   	 }	

 
}
