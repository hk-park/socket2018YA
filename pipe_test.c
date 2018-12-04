#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
	int fd[2], fd2[2]; //pipe로 사용할 파일디스크립터
	char buf[255];
	int pid;

	if(pipe(fd) < 0 || pipe(fd2) < 0){ //pipe 생성. 생성 실패 시, 프로그램 종료
		printf("[ERROR] pipe error\n");
		exit(0);
	}
	pid = fork();
	if(pid > 0){
		//부모 프로세스
		//자식프로세스가 파이프를 통해 전달하는 값을 출력
		memset(buf, 0x00, 255);
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] child message: %s\n", buf);
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm parent.", getpid());
		write(fd2[1], buf, strlen(buf));
	}else if (pid == 0){
		//자식 프로세스
		//파이프를 통해 부모 프로세스에게 값을 전달
		memset(buf, 0x00, 255);
		sprintf(buf, "[%d] Hello, I'm child.", getpid());
		write(fd[1], buf, strlen(buf));
		memset(buf, 0x00, 255);
		read(fd2[0], buf, sizeof(buf));
		printf("[CHILD] parent message: %s\n", buf);
		
	}else{
		printf("[ERROR] fork() failed\n");
		exit(0);
	}
}
