#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(){
	int fd[2];
	char buf[BUFSIZ];
	int pid;

	if(pipe(fd) < 0){
		printf("[ERROR] pipe 생성 실패\n");
		exit(0);
	}

	pid = fork();

	if(pid > 0){
		//parent
		memset(buf, 0, BUFSIZ);
		read(fd[0], buf, sizeof(buf));
		printf("[PARENT] (%s) received.\n", buf);
	} else if(pid == 0){
		//child
		memset(buf, 0, BUFSIZ);
		sprintf(buf, "[%d]Hello, I'm child.", getpid());
		write(fd[1],buf, strlen(buf));
	} else {
		//error
		printf("[ERROR] fork() 실패");
	}
}
