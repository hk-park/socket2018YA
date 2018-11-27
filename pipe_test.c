#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 100

int main()
{
	int fd[2]; 	 // 파일 디스크립터 2개 생성
	char buf[BUFSIZE]; // 입력 메세지를 받기위한
	int pid;

	if (pipe(fd) < 0) 	// pipe 생성 실패 시, 프로그램 종료
	{
		printf("[ERROR] Pipe 생성 실패\n");
		exit(0);
	}

	pid = fork();
	if (pid > 0)
	{	// 부모 프로세스
		// 자식 프로세스가 보낸 메세지 읽어서 출력
		memset(buf, 0x00, BUFSIZE);
		read(fd[0], buf, sizeof(buf));	// read - 0
		printf("[PARENT] (%s) received.\n", buf);
	}
	else if (pid == 0)
	{	// 자식 프로세스
		// 부모 프로레스에게 메세지를 전달
		memset(buf, 0x00, BUFSIZE);	// buf 배열 초기화
		
		sprintf(buf, "[%d] Hello, I'm Child !", getpid());
		write(fd[1], buf, strlen(buf));	// pipe는 양방향임 (write = 1, read = 0)
	}
	else 
	{	// 생성 실패
		printf("[ERROR] fork 실패 \n");
	}


	return 0;
}
