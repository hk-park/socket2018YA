#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 255
int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;
		
	if(pipe(fd) < 0){ // 파이프생성. 비정상적인 실행(실패)은 -1 반환.
		printf("[ERROR] pipe 생성 실패\n");
		exit(0);
	}
	pid = fork();
	if(pid > 0 ){ // 부모
		// 자식프로세스가 보낸 메시지를 읽어서 출력
		memset(buf, 0x00, BUFSIZE);
		read(fd[0], buf, sizeof(buf));
		printf("[Parent] I took this: %s\n", buf);
	} else if (pid == 0){ // 자식
		// 부모프로세스에게 메시지를 전달
		memset(buf, 0x00, BUFSIZE);
		sprintf(buf, "[%d] Hello, I'm child.", getpid());
		//write(c_socket, buf, strlen(buf));
		write(fd[1], buf, strlen(buf)); //라이트 fd[1], 리드  fd[0]
	} else { // 생성실패
		printf("[ERROR] fork() 실패\n");
	}
}
