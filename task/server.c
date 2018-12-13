#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 511
#define MAX_SOCK 50

char* EXIT_STRING = "exit";	// 종료 요청 문자열
char* START_STRING = "Conneted to char_server\n";//클라이언트 환영 메세지

int maxfdp1;			// 최대 소켓 번호 +1
int num_chat = 0;		// 채팅 참가자 수
int clisock_list[MAX_SOCK];// 채팅에 참가자 소켓번호 목록
int listen_sock;			// 서버의 리슨 소켓
// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in* newcliaddr);
int getmax();			// 최대 소켓 번호 찾기
void removeClient(int s);	// 채팅 탈퇴 처리 함수
int tcp_listen(int host, int port, int backlog);
// 소켓 생성 및 listen
void errquit(char *mesg) { perror(mesg); exit(1);}

typedef struct user {
	int sock;
	char nickname[50];
} data;
data userdata[MAX_SOCK];
int main(int argc , char* argv[]) {
	struct sockaddr_in cliaddr;
	char buf[MAXLINE+1];
	int i,j,nbyte, accp_sock;
	int addrlen = sizeof(struct sockaddr_in);
	fd_set read_fds;	// 읽기를 감지할 fd_set 구조체
	char nickname[50];

	if(argc !=2 )	{
		printf("사용법 : %s port: \n",argv[0]);
		exit(1);
	}
	// tcp_listen(host, port, backlog) 함수 호출
	listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]),5);

	while(1) {
		FD_ZERO(&read_fds);	// 모든 비트를 지움
		FD_SET(listen_sock,&read_fds); // 초기 서버소켓(클라이언트 참가용)을 선택
		for(i=0;i<num_chat;i++)
			FD_SET(clisock_list[i],&read_fds);	// 참가중인 모든 클라이언트의 소켓을 선택

		maxfdp1 = getmax() +1;	// maxfdp1 재 계산
		puts("wait for clint");
		if(select(maxfdp1,&read_fds,NULL,NULL,NULL) < 0 )
			errquit("select fail");

		if(FD_ISSET(listen_sock,&read_fds))
		{	// 초기 서버소켓 listen_sock 에서 입력 발생
			accp_sock = accept(listen_sock,(struct sockaddr*) &cliaddr,&addrlen);
			if(accp_sock==-1) errquit("accept fail");

			addClient(accp_sock, &cliaddr);
			send(accp_sock,START_STRING, strlen(START_STRING), 0 );
			read(accp_sock,nickname,50);
			printf("접속자 %d명 : %s님이 접속하셨습니다.\n",num_chat,nickname);
			strcpy(userdata[num_chat-1].nickname,nickname);	// 닉네임을 구조체에 저장
			userdata[num_chat-1].sock=accp_sock;	// 소켓fd를 구조체에 저장
			printf("%d소켓 %d번째에 저장\n",accp_sock,num_chat-1);
			continue;
		}
		// 클라이언트가 보낸 메세지를 모든 클라이언트에게 방송
		for(i=0;i < num_chat; i++)	{
			if(FD_ISSET(clisock_list[i],&read_fds)) {
				// 통신용 소켓 clisock_list[i]에서 채팅 메세지 수신
				nbyte =recv(clisock_list[i],buf , MAXLINE, 0);
				if(nbyte <= 0) {
					removeClient(i);	// 클라이언트의 종료
					continue;
				}
				buf[nbyte] = 0;
				// 종료문자 처리
				if(strstr(buf,EXIT_STRING) != NULL) {
					removeClient(i);	// 클라이언트의 종료
					continue;
				}

				if(strstr(buf,"/w") != NULL) {	// 귓속말일때
					char nick[50];
					strtok(buf,"/w: ");
					sprintf(nick, "%s",strtok(NULL,":/w "));	// /w 아이디 메세지내용, 문자열 파싱
					for(j=0;j<num_chat;j++) if(!strcmp(userdata[j].nickname,nick)) break;	// 같은 닉네임을 찾는다.
					sprintf(buf, "%s :%s",buf,strtok(NULL,"") );
					printf("[To %s, From %s\n",nick, buf);	// 서버에 출력
					send(userdata[j].sock,buf,strlen(buf),0);	// 귓속말 닉네임과 같은 닉네임의 구조체의 소켓에 전송
				} else {	// 모든 채팅 참가자에게 메세지 방송
					for(j=0;j<num_chat;j++)
						send(clisock_list[j],buf,nbyte,0);
					printf("%s\n",buf);
				}
			}
		}
	}
	return 0;
}
// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in* newcliaddr) {
	char buf[20];
	inet_ntop(AF_INET,&newcliaddr->sin_addr,buf,sizeof(buf));
	printf("new client : %s\n",buf);
	clisock_list[num_chat] = s;
	num_chat++;	// 현재 참가자수를 늘림
}
// 채팅 탈퇴 처리
void removeClient(int s){
	int i;
	for(i=0;i<50;i++) if(userdata[i].sock == clisock_list[s]) break;
	userdata[i]=userdata[num_chat-1];	// 배열 overflow방지.
	// 마지막데이터를 탈퇴한 구조체 배열에 넣는다.

	close(clisock_list[s]);
	if(s != num_chat-1) {
		clisock_list[s] = clisock_list[num_chat-1];
	}
	num_chat--;	// 현재 참가자 수를 줄임
	printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d \n",num_chat);

}
// 최대 소켓번호 찾기
int getmax()
{
	int max= listen_sock;
	int i;
	for(i=0; i<num_chat;i++)
		if(clisock_list[i] > max)
			max=clisock_list[i];
	return max;
}
// listen 소켓 생성 및 listen
int tcp_listen(int host, int port, int backlog)
{
	int sd;
	struct sockaddr_in servaddr;
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0) {
		perror("socket fail"); exit(1);
	}
	// servaddr 구조체의 내용 세팅
	bzero((char*) &servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(host);
	servaddr.sin_port = htons(port);
	if(bind(sd,(struct sockaddr*) &servaddr,sizeof(servaddr)) < 0)
	{perror("bind fail"); exit(1);}
	// 클라이언트로부터 연결요청을 기다림
	listen(sd,backlog);
	return sd;
}
