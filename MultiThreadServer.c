#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#define PORT 9000
#define BUFSIZE 100
void *do_service(void* data); 
char buffer[BUFSIZ] = "Hi, I'm server\n";

main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	
	pthread_t pthread;
	int thr_id;

 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
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
		thr_id=pthread_create(&pthread,NULL,do_service,(void *)&c_socket);
		}
}
void * do_service(void *data){
	int n;
	char rcvBuffer[BUFSIZE];
	
	int c_socket=*((int *)data);

	while(1){
		if(readSize=read(c_socket,rcvBuffer,sizeof(rcvBuffer))<0){
		
		}
	}
