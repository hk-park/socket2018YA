#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 100

int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;
	
	if(pipe(fd) < 0){ // pipe생성, 생성실패시 프로그램 종료
		printf("[ERROR MESSAGE] Pipe 생성 실패\n");
		exit(0);
	}

	pid = fork();
	
	if(pid>0){
		memset(buf, 0, BUFSIZE);
		read(fd[0], buf, BUFSIZE);
		printf("[PARENT] (%s) received.\n", buf);
	}
	else if(pid==0){
		memset(buf, 0x00, BUFSIZE);
		sprintf(buf, "[%d] : Hello, Im child.", getpid());
		write(fd[1], buf, strlen(buf));
	}
	else{
		printf("[ERROR MESSAGE] fork() 실패 \n");
	}

	return 0;
}
