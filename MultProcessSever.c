#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 9000
 
char buffer[BUFSIZ] = "****************\nMy name is subin\n****************\n";
char buffer2[BUFSIZ] = "****************\nfile list\n****************\n";
char buffer3[BUFSIZ] = "****************\ncomand not foundn\n****************\n";

 
char rBuffer[BUFSIZ];
 
void do_service(int c_socket);
void sig_handler(int signo);
main(){

    DIR *dp;
    struct dirent *dir;


    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int len;
    int n, i;

    char *temp;
    int length;
	signal(SIGCHLD, sig_handler);
    s_socket = socket(PF_INET, SOCK_STREAM, 0);

    memset(&s_addr,0,sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
 
    if(bind(s_socket,(struct sockaddr *)&s_addr, sizeof(s_addr))==-1){
        printf("Can Not Bind\n");
        return -1;
    }
 
    if(listen(s_socket, 5)==-1){
        printf("listen Fail\n");
        return -1;
    }
 
    while(1){
        len = sizeof(c_addr);
        c_socket = accept(s_socket,(struct sockaddr *)&c_addr, &len);

        length=0;
        temp=rBuffer;
     
        while((n=read(c_socket, temp, 1))>0){
            if(*temp == '\r') continue;
            if(*temp == '\n') break;
            if(*temp == '\0') break;

            if(length == BUFSIZ) break;

            temp++; length++;
        }
     
        rBuffer[length]='\0';
     
        if(!strcmp(rBuffer, "print")){
            n = strlen(buffer);
            write(c_socket, buffer, n);
        } 
     
        else if(!strcmp(rBuffer, "ps")){
            n = strlen(buffer2);
            write(c_socket, buffer2, n);
        } 

        else if(!strcmp(rBuffer, "list")){
            if((dp=opendir("."))==NULL){
                fprintf(stderr, "directory open error\n");
                exit(-1);
            }

            while((dir=readdir(dp))!=NULL){
                if(dir->d_ino==0)    continue;
                printf("%s\n",dir->d_name);
            }
        }
        else {
            n = strlen(buffer3);
            write(c_socket, buffer3, n);
        } 
    }
void sig_handler(int signo){
		int pid;
		int status;
		pid=wait(&status);//자식 프로세스가 종료될 때까지 기다려주는 함수
		printf("pid[%d] process terminated.status \n",pid, status);
}
