#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib,h>

#define BUFSIZ 100

int main(){
	int fd[2];
	char buffer[BUFSIZ];
	int pid;

	if(pipe(fd) < 0){ //pipe 생성부,  파이프 생성 실패 시, 프로그램 종료
			printf("[ERROR] : PIPE CREATE FAILED.\n");
			exit(0);
	}
	
	pid = fork();
	if(pid > 0){ //부모 프로세스
		//자식 프로세스가 보낸 메세지를 읽어서 출력
		memset(buf,0x00,BUFSIZ);
		read(fd[0],buf,sizeof(buf));
		printf("[PARENT] (%s) recived.\n",buf);
	}

	else if(pid == 0){ //자식 프로세스
		//부모 프로세스에 메세지 전달
		memset(buf,0x00,BUFSIZ);
		sprintf(buf,"[%d] HELLO, I'M CHILED.\n",getpid());
		//write(c_socket, buf, strlen(buf));
		write(pipe[1], bug, strlen(buf));
	}
	
	else{ //생성 실패
		printf("[ERROR] : FORK() FAILED.\n");
		
	}
