#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 10000

char buffer[100] = "Hello World!\n";

int main(void)
{
	int c_socket; //client socket
	int s_socket; //server socket
	struct sockaddr_in s_addr, c_addr;
   int   len;
   int   n,a,x;
	FILE *fp;
   int rcvLen;
	char buffer2[100];
   char rcvBuffer[100],*jdy1,*jdy2;
   char jdyBuffer[100] = " ";
	
	s_socket = socket(PF_INET, SOCK_STREAM, 0); //create server socket
	
	memset(&s_addr, 0, sizeof(s_addr)); //initionaize server socket
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //distribute(assign) IP
	s_addr.sin_family = AF_INET; //which 
	s_addr.sin_port = htons(PORT); 


	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1){
		printf("Cannot bint\n"); //failed to bind
		return -1;
	}

	if(listen(s_socket, 5) == -1){
		printf("listen failed\n");
		return -1; //error
	}
	while(1) //actual procces
	{
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		//able to communicate
		if((pid = fork()) > 0) {// 부모 프로세스
		 // 다른 클라이언트의 요청 접수
		   close(c_socket);
		   continue;

		} else if(pid == 0) {// 자식 프로세스:
		   // 클라이언트의 요청 처리
		   close(s_socket);
		   do_service(conn_s);
		   exit(0);
		}
		n = strlen(buffer);
		write(c_socket, buffer, n); //send
		close(c_socket);
	}
	close(s_socket); //close server socket

}

do_service(int c_socket) {
        while(1) {
                len = sizeof(c_addr);
                c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
                printf("Client is connected\n");
                while(1){
                        rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
                        rcvBuffer[rcvLen] = '\0';
			buffer[0] ='\0';
                        printf("[%s] received\n", rcvBuffer);
                        if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
                        {
                                break;
                        }
                        else if(!strncmp(rcvBuffer, "안녕하세요",strlen("안녕하세요")))
                          {
                                strcpy(buffer,"안녕하세요 만나서 반가워요 .");
                                n=strlen(buffer);
                                write(c_socket,buffer,n);
                          }
                        else if(!strncmp(rcvBuffer,"이름이 머야?",strlen("이름이 머야?")))
                         {
                                strcpy(buffer,"내이름을 정달영이야");
                                n = strlen(buffer);
                                write(c_socket,buffer,n);
                         }
                        else if(!strncmp(rcvBuffer,"몇 살이야?",strlen("몇 살이야?")))
                         {
                                strcpy(buffer,"나는 100살이야.");
                                n =strlen(buffer);
                                write(c_socket,buffer,n);
                        }

                        else if(!strncasecmp(rcvBuffer,"strlen ",7))
                        {
                         a  = strlen(rcvBuffer)-7;
                         sprintf(buffer,"길이 : %d",a);
                         write(c_socket, buffer,strlen(buffer));
                        }

                        else if (!strncasecmp(rcvBuffer,"strcmp ",7))
                        {
                          strtok(rcvBuffer," ");
                          jdy1=strtok(NULL," ");
                          jdy2=strtok(NULL," ");
                          x=strcmp(jdy1,jdy2);

                          sprintf(buffer,"%d\n",x);
                          write(c_socket,buffer,strlen(buffer));

                        }
		else if (!strncasecmp(rcvBuffer,"readfile",8))
			{
				strtok(rcvBuffer," ");
				jdy1 = strtok(NULL," ");
				jdy2 = jdy1;
				fp=fopen(jdy2,"r");	
				
				if(fp)
				{
					while(fgets(buffer2,100,(FILE *)fp))
						strcat(buffer, buffer2);
				}
				fclose(fp);
				write(c_socket,buffer,strlen(buffer));

			}

			else if (!strncasecmp(rcvBuffer,"exec",4))
					{

							strtok(rcvBuffer," ");
							jdy1 = strtok(NULL,"NULL");
							jdy2 = jdy1;
							int ret = system(jdy2);
							
							if(!ret)
							  sprintf(buffer,"%s,command is Success",jdy2);
							else
							  sprintf(buffer,"%s,command is failed",jdy2);
							 n =strlen(buffer);
                         				write(c_socket, buffer, n);
						
	             }
 				}
	                    else{
                        n =strlen(buffer);
                        write(c_socket, jdyBuffer, n);
                        }

                  close(c_socket);
                if(!strncasecmp(rcvBuffer, "kill server", 11))
                        break;
        }
        close(s_socket);
}
