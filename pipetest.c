#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 100
int main(){
	int fd[2];
	char buf[BUFSIZE];
	int pid;

	if(pipe(fd) <0){//pipe 생성,생성실패시 프로그램종료
		printf("[ERROR] pipe생성실패 \n");
		exit(0);
	}
	pid = fork();
	if(pid>0){
		memset(buf,0x00,BUFSIZE);
		read(fd[0],buf,sizeof(buf));
		printf("[PARENT] (%s)received \n",buf);
	}
	else if(pid ==0){//자식 프로세스
		memset(buf,0x00,BUFSIZE);
		sprintf(buf,"[%d] Hello, I'm child",getpid());
		write(fd[1],buf,strlen(buf));
		//부모프로세스에게 메세지전달
	}
	else{
		printf("{ERROR] fork() fall \n");
		exit(0);
	}
}
