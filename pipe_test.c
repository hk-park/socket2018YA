#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define BUFSIZE 100

int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;

	if(pipe(fd) < 0){
		printf("[ERROR] Pipe 생성실패 \n");
		exit(0);
	}
	pid = fork();

	if( pid > 0){ //부모프로세스
		memset(buf,0x00,BUFSIZE);
		read(fd[0],buf,strlen(buf)); //buf 불러오기
		printf("[PARENT] (%s) received.\n",buf);
	}else if(pid == 0){//자식프로세스 
		memset(buf,0x00, BUFSIZE);
		sprintf(buf,"[%d] Hello, I'm child.", getpid());
		write(fd[1],buf, strlen(buf)); // buf에 문자 입력
	}else{
		printf("[ERROR] fork() 실패\n");
	}
}
