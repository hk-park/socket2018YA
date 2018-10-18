#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define BUFSIZE 100
int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;

	if(pipe(fd) < 0){//pipe 생성, 생성실패시 프로그램 종료
	printf("[ERROR] pipe fail\n");
	exit(0);
	}
	pid = fork();
	if(pid > 0){ //부모프로세스
		memset(buf, 0x00, BUFSIZE);	//자식 프로세스가 보낸 메세지를 읽어서 출력
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] (%s) received.\n",buf);
	}else if(pid == 0){//자식프로세스
		memset(buf, 0x00, BUFSIZE);	//부모프로세스에게 메세지 전달
		sprintf(buf, "[%d] Hello, I'm child.", getpid());
		write(fd[1], buf, strlen(buf));
	}else{ //생성실패
		printf("[ERROR] fork fail\n");
		exit(0);
	}
}
