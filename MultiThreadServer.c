#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

// 2-1. 서버 프로그램이 사용하는 포트
#define PORT 9000
#define BUFSIZE 10000
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정

void *do_chat(void *);
int pushClient(int);
int popClient(int);

pthread_t thread;
pthread_mutex_t mutex;

#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define NAME_LENGTH 30

struct user
{
	int c_socket;
	char u_name[NAME_LENGTH];
};

int clientCount = 0;
char escape[ ] = "exit";
char greeting[ ] = "Welcome to chatting room.\n";
char CODE200[ ] = "Sorry No More Connection. \n";
struct user list_user[MAX_CLIENT];

int main(int argc, char *argv[ ])
{
	int c_socket, s_socket;		// 변수 선언
	struct sockaddr_in s_addr, c_addr;	// 클라이언트에서 보낸 데이터 받음
	int i, j, n, res, len;

	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		printf("Can not create mutex \n");
		return -1;
	}

	s_socket = socket(PF_INET, SOCK_STREAM, 0);			// 서버 소켓 생성	

	memset(&s_addr, 0, sizeof(s_addr));					// 어떤 port와 어떤 ip를 사용할 것인지 
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// ip 주소
	s_addr.sin_family = AF_INET;						// 네트워크 방식 (IPv4) IPv4 - 32비트, 8비트씩 4부분으로 10진수로 표시, IPv6 - 128비트, 16비트씩 8부분으로 16진수로 표시
	s_addr.sin_port = htons(PORT);						// port 번호, htons = 네트워크가 이해할 수 있는 것으로 변형	

	if (bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {	// bind 시작, 정보 값 (port가 사용 중이라면)
		printf("Can not Bind\n");
		return -1;
	}

	if (listen(s_socket, 5) == -1) {	// listen 소켓 커널에 개통 요청, 동접자 5명
		printf("listen Fail\n");
		return -1;
	}

	for (i = 0; i < MAX_CLIENT; i++)
		list_user[i].c_socket = INVALID_SOCK;	

	while (1)
	{
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		res = pushClient(c_socket);
		if (res < 0)
		{
			write(c_socket, CODE200, strlen(CODE200));
			close(c_socket);
		}
		else
		{
			write(c_socket, greeting, strlen(greeting));
			pthread_create(&thread, NULL, do_chat, (void *)&c_socket);
		}
	}
	close(s_socket);
}

int pushClient(int c_socket)
{
	if (clientCount >= MAX_CLIENT)
	{
		printf("클라이언트가 최대 수를 초과하였습니다.\n");
		return -1;
	}
	else
	{
		pthread_create(&thread, NULL, do_chat, (void *)&c_socket);
		pthread_mutex_lock(&mutex);
		list_user[clientCount++].c_socket = c_socket;
		pthread_mutex_unlock(&mutex);
		printf("%d 개의 클라이언트가 접속하였습니다.\n", clientCount);
	}
}

int popClient(int c_socket)
{
	pthread_mutex_lock(&mutex);
	list_user[--clientCount].c_socket = INVALID_SOCK;
	pthread_mutex_unlock(&mutex);
	close(c_socket);
	printf("현재 %d개의 클라이언트가 남아있습니다.\n", clientCount);
	return -1;
}

void *do_chat(void *arg)
{
	int c_socket = *((int *)arg);
	char chatData[CHATDATA], rcvChatData[CHATDATA];
	int i, len;
	while(1)
	{
		memset(chatData, 0, sizeof(chatData));
		if ((len = read(c_socket, chatData, sizeof(chatData))) > 0)
		{
			// wirte chatData to all clients
			rcvChatData[len] = '\0';
			if (!strncasecmp(rcvChatData, "w ", 2))
			{
				int i = 0, n = 0;
				char *str[3];
				char *token = strtok(rcvChatData, " ");
				while(token != NULL)
				{
					str[i++] = token;
					token = strtok(NULL, " ");
				}

				for (n = 0; n < clientCount; n++)
				{
					if (!strcmp(list_user[n].u_name, str[1]))
					{
						write(list_user[n].c_socket, str[2], strlen(str[2]));
						break;
					}
				}
			}
			else
			{
				for(i = 0; i < clientCount; i++)
				{
					write(list_user[i].c_socket, chatData, strlen(chatData));
				}
			}
			if (strstr(chatData, escape) != NULL)
			{
				popClient(c_socket);
				break;
			}
		}
	}
}
