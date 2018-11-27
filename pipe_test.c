#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define BUFSIZE 100

int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;

	if(pipe(fd) < 0){ //pipe 생성, 생성 실패시, 프로그램 종료
		printf("[ERROR] Pipe 생성실패\n");
		exit(0);
	}
	pid =fork();
	if(pid > 0){
		
	}else if(pid==0){
		// 부모프로세스에게 메세지 전달
		memset(buf, 0x00, BUFSIZE);
		sprintf(buf, "[%d] Gello, I'm child.",getpid());
		write(fd[1], buf, strlen(buf));
		
	}else{
		printf("[ERROR] fork()실패\ n");
		exit(0);
	}
	

