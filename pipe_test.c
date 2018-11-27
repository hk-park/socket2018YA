#include <stdio.h>
#include <unistd.h> //pipe를 쓸려면 필요
#include<string.h>
#include<stdlib.h>

#define BUFSIZE 100
int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;

	if(pipe(fd) < 0){//pipe 생성, 생성 실패 시, 프로그램을 종료
		printf("[ERROR] pipe 생성 실패\n");
		exit(0);
	}
	pid = fork();
	if(pid>0){ //부모 프로세스
		//자식 프로세스가 보낸 메세지를 읽어서 출력
		memset(buf, 0x00, BUFSIZE);
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] (%s) received.\n", buf);
	}else if (pid == 0){ //자식프로세스
		// 부모 프로세스에게 메세지를 전달
		memset(buf, 0x00, BUFSIZE);
		sprintf(buf, "[%d] Hello. I'm child.", getpid()); 
		//write(c_socket, buf, strlen(buf));
		write(fd[1], buf, strlen(buf));

	}else { //생성 실패
		printf("[ERRPR] fork() 실패\n");
		exit(0);
} 
}
