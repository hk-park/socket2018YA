#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 10000

void doService(int c_socket);

int main(void){
	int s_socket, c_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int pid;
	int stat=0;

	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = inet_addr(IP);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1){
		printf("[ERROR] Cannot bind.\n");
		return -1;
	}

	if(listen(s_socket, 5)==-1){
		printf("[ERROR] Listen failed.\n");
		return -1;
	}
	while(1){
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);


		pid = fork();
		if(pid>0){
			close(c_socket);
			//stat++;
			//printf("현재 %d개의 클라이언트가 접속 중입니다.\n", stat);
			continue;
		}
		else if(pid==0){
			close(s_socket);
			doService(c_socket);
			exit(0);
			if(stat==-1)
				break;
		}
	}
	close(s_socket);

	return 0;
}

void doService(int c_socket){
	int rcvlen;
	int itemp;
	char buffer[100];
	char rcvbuf[100];
	char tmpbuf1[100];
	char tmpbuf2[100];
	char *tmptoken;
	FILE* fp;

	printf("==== Client is connected. ====\n");
	strcpy(buffer, "==== Server is connected. ====\n");
	write(c_socket, buffer, strlen(buffer));

	while(1){
		strcpy(buffer, "\0");
		rcvlen = read(c_socket, rcvbuf, sizeof(rcvbuf));
		rcvbuf[rcvlen] = '\0';
		printf("[%s] recieved.\n", rcvbuf);

		if(strcmp(rcvbuf, "안녕하세요")==0){
				strcpy(buffer, "안녕하세요. 만나서 반가워요");
				buffer[strlen(buffer)] = '\0';
				write(c_socket, buffer, strlen(buffer));
			}

			else if(strcmp(rcvbuf, "이름이 머야?")==0){
				strcpy(buffer, "내이름은 김영민이야.");
				buffer[strlen(buffer)] = '\0';
				write(c_socket, buffer, strlen(buffer));
			}

			else if(strcmp(rcvbuf, "몇살이야?")==0){
				strcpy(buffer, "나는 24살이야.");
				buffer[strlen(buffer)] = '\0';
				write(c_socket, buffer, strlen(buffer));
			}

			else if(strncasecmp(rcvbuf, "strlen", 6)==0){
				sprintf(buffer, "길이=%d", strlen(rcvbuf)-7);
				write(c_socket, buffer, strlen(buffer));
			}

			else if(strncasecmp(rcvbuf, "strcmp", 6)==0){
				strtok(rcvbuf, " ");
				strcpy(tmpbuf1, strtok(NULL, " "));
				strcpy(tmpbuf2, strtok(NULL, " "));
				if(strcmp(tmpbuf1, tmpbuf2)==0){
					strcpy(buffer, "문자열 일치(값=0)");
				}
				else{
					sprintf(buffer, "문자열 불일치(값=%d)", strcmp(tmpbuf1, tmpbuf2));
				}
				write(c_socket, buffer, strlen(buffer));
			}

			else if(strncasecmp(rcvbuf, "readfile", 8)==0){
				strtok(rcvbuf, " ");
				strcpy(tmpbuf1, strtok(NULL, " "));
				
				fp = fopen(tmpbuf1, "r");
				if(fp){
					while(fgets(buffer, 100, (FILE*) fp)){
						write(c_socket, buffer, strlen(buffer));
					}
				}
				else{
					strcpy(buffer, "파일을 찾을 수 없습니다.");
					write(c_socket, buffer, strlen(buffer));
				}
			}

			else if(strncasecmp(rcvbuf, "exac", 4)==0){
				strtok(rcvbuf, " ");
				strcpy(tmpbuf1, "\0");
				while(1){
					tmptoken = strtok(NULL, " ");
					if(tmptoken==NULL) break;
					strcat(tmpbuf1, tmptoken);
				}
				itemp = system(tmpbuf1);
				if(!itemp){
					sprintf(buffer, "%s command is exacuted.", tmpbuf1);
				}
				else{
					sprintf(buffer, "%s command failed.", tmpbuf1);
				}
				write(c_socket, buffer, strlen(buffer));
			}
		else if(strncmp(rcvbuf, "quit", 4)==0 || strncmp(rcvbuf, "kill server", 11)==0)
			break;
		else{
			write(c_socket, "반응없음", 12);
		}
	}
	close(c_socket);
}
