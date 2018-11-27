#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define BUFSIZE 100
int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;
	if(pipe(fd) < 0){	
		//pipe 생성. 생성 실패시. 프로그램 종료
		printf("[ERROR] Pipe 생성 실패\n");
		exit(0);
	}
	pid = fork();
	if(pid > 0)
	{
		//부모 프로세스
		memset(buf, 0x00, BUFSIZE);
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] (%s) received. \n", buf);
	}else if(pid==0){
		//자식 프로세스
		//부모 프로세스에게 메세지를 전달
		memset(buf, 0x00, BUFSIZE);
		sprintf(buf, "[%d] Hello, I'm child", getpid());
		//socket : write(c_socket, buf, strlen(buf));
		write(fd[1], buf, strlen(buf));
		//write일때는 항상 fd[1]. read일 때는 항상 fd[0] (파이프는 양방향이라서 이렇게 선언했음)
	}else{
		//생성 실패
		printf("[ERROR] fork() failed\n");
	}


}
