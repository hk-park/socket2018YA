#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <pthread.h>

pthread_t tid;
#define MAXLINE 1000
#define NAME_LEN 20
char* EXIT_STRING ="exit";
int tcp_connect(int af, char* servip, unsigned short port);
// 소켓 생성 및 서버 연결, 생성된 소켓 리턴
void errquit(char* mesg){perror(mesg); exit(1);}

// 메세지를 수신하는 쓰레드
void *thr_receive(void *arg) {
	int s=(int)arg;
	char *bufmsg;
	int nread;

	while(1) {
		if( (nread = recv(s,bufmsg,MAXLINE,0)) > 0) {	// 메세지를 수신한다.
			bufmsg[nread] = 0;
			fputs(bufmsg,stdout);
		}
	}
	return (void*)0;
}
int main(int argc, char* argv[])
{
	char bufall[MAXLINE+NAME_LEN];	// 이름 + 메세지를 위한 버퍼
	char* bufmsg;					// bufall에서 메세지부분의 포인터
	int maxfdp1;					// 최대 소켓 디스크립터
	int  s;							// 소켓
	int namelen;					// 이름의 길이
	fd_set read_fds;
	char nickname[50];

	if(argc !=3) {
		printf("manual : %s server_ip port\n", argv[0]);
		exit(1);
	}
	printf("닉네임을 입력하시오 : ");	// 닉네임을 입력받는다.
	scanf("%s",nickname);
	fflush(NULL);
	sprintf(bufall, "%s :", nickname);	// bufall의 앞쪽에 닉네임저장
	namelen = strlen(bufall);
	bufmsg = bufall + namelen;
	s = tcp_connect(AF_INET, argv[1], atoi(argv[2]));//메세지 시작위치 지정, 소켓생성
	pthread_create(&tid,NULL,thr_receive,(void*)s);	//쓰레드 생성
	if(s==-1)
		errquit("tcp_connect fail");
	puts("서버에 접속되었습니다.");
	send(s, nickname, 50, 0);	// 닉네임을 서버에 전송
	int c;
	while ((c=getchar()) != EOF && c != '\n');	// 버퍼를 비워준다.


	while(1)
	{
		if(fgets(bufmsg,MAXLINE,stdin)) {	// 입력 받고
			if(send(s, bufall, namelen + strlen(bufmsg), 0) < 0)	// 서버에 보낸다.
				puts("error write on socket");
			if(strstr(bufmsg, EXIT_STRING) != NULL)	// exit가 있으면 종료한다.
			{
				puts("good bye");
				close(s);
				exit(0);
			}
		}
	}	// end of while
	return 0;
}

int tcp_connect(int af, char* servip, unsigned short port)
{
	struct sockaddr_in servaddr;
	int s;	// 소켓 생성
	if((s = socket(af, SOCK_STREAM, 0)) < 0)
		return -1;
	// 채팅 서버의 소켓주소 구조체 sevaddr 초기화
	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = af;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);
	// 연결요청
	if(connect(s, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		return -1;
	return s;
}
