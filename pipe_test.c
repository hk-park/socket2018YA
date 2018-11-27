#include <stdio.h>
#include <unistd.h> //파이프 쓰기 위해
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 100
int main(){
	int fd[2]; //파이프를 생성해서 리턴받을 파일 디스크립터 두개 생성
	char buf[BUFSIZE]; //다른 메세지를 읽을 버퍼
	int pid;
	
	if(pipe(fd)<0){//pipe 생성, 생성 실패시 프로그램 종료
		printf("[ERROR] Pipe 생성 실패\n");
		exit(0);
	}
	pid = fork();

	if(pid>0){ //부모 프로세스
		//자식 프로세스가 보낸 메세지를 읽어서 출력
		memset(buf, 0x00, BUFSIZE);
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] (%s) received.\n", buf);	
	}else if(pid==0){ //자식 프로세스
		//부모 프로세스에게 메세지를 전달
		memset(buf, 0x00, BUFSIZE);
		sprintf(buf, "[%d] Hello, I'm child.", getpid());
		//write(c_socket, buf, strlen(buf)); 요건 소켓에서의 write
		write(fd[1], buf, strlen(buf)); //write할 땐 fd[1] read할 땐 fd[0]	
	}else{ //생성 실패
		printf("[ERROR] fork() 실패\n");
		exit(0);
	}
}
