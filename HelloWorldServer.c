
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
//#define PORT 9000
#define PORT 10000

// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";


main( )
{
  char rcvbuf[100];
  char buffer1[100];
  char buffer2[100];
  char buffer[100];
  int   c_socket, s_socket;
  struct sockaddr_in s_addr, c_addr;
  int   len;
  int   n;
  int rcvLen;
  char rcvBuffer[100];
  s_socket = socket(PF_INET, SOCK_STREAM, 0);
  
  memset(&s_addr, 0, sizeof(s_addr));
  //s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(PORT);
  
  if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
    printf("Can not Bind\n");
    return -1;
  }
  
  if(listen(s_socket, 5) == -1) {
    printf("listen Fail\n");
    return -1;
  }
  
  while(1) {
    len = sizeof(c_addr);
    c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
    //3-3.클라이언트가 접속했을 때 "Client is connected" 출력
    printf("Client is connected\n");
    
    while(1){
      rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
      rcvBuffer[rcvLen] = '\0';
      
      printf("[%s] received\n", rcvBuffer);
      
      if(strcmp(rcvBuffer,"안녕하세요")==0){
        strcpy(buffer, "안녕하세요. 만나서 반가워요");
        
        write(c_socket,buffer,strlen(buffer));
      }else if(strcmp(rcvBuffer,"이름이 머야?")==0){
        strcpy(buffer, "내 이름은 김영민이야");
        buffer[strlen(buffer), strlen(buffer)]='\0';
        write(c_socket,buffer,strlen(buffer));
      }else if(strcmp(rcvBuffer,"몇 살이야?")==0){
        strcpy(buffer, "나는 24살이야");
        buffer[strlen(buffer), strlen(buffer)]='\0';
        write(c_socket,buffer,strlen(buffer));
      }else if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
      break;
      else if(strncasecmp(rcvBuffer,"strlen",6)==0){
        sprintf(buffer,"length:%d",strlen(rcvBuffer)-7);
        write(c_socket,buffer,strlen(buffer));
      }else if(strncasecmp(rcvBuffer,"strcmp",6)==0){
        strtok(rcvBuffer,"");
        strcpy(buffer1,strtok(NULL,""));
        strcpy(buffer2,strtok(NULL,""));
        if(strcmp(buffer1,buffer2)==0){
          strcpy(buffer,"문자열 일치(값:0)");
          write(c_socket,buffer,strlen(buffer));
        }else{
          sprintf(buffer,"문자열 불일치(값:%d)",strcmp(buffer1,buffer2));
          write(c_socket,buffer,strlen(buffer));
        }
      }
    }
    close(c_socket);
    if(!strncasecmp(rcvBuffer, "kill server", 11))
    break;
  }	
  close(s_socket);
}
