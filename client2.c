#include <stdio.h>
 #include <netinet/in.h>
 #include <sys/socket.h>
 #include <string.h>
 
 #define PORT 9001
 #define IP_SERVER "127.0.0.1"
 
 char buffer[BUFSIZ];
 
 int  main()
 {
         int c_socket,s_socket;
         struct sockaddr_in c_addr;
         int len;
         int n;
         char rcvBuffer[100];
         char sendBuffer[100];
 
         c_socket=socket(PF_INET,SOCK_STREAM,0);
 
         memset(&c_addr,0,sizeof(c_addr));
         c_addr.sin_addr.s_addr=inet_addr(IP_SERVER);
         c_addr.sin_family=AF_INET;
         c_addr.sin_port=htons(PORT);
 
         if(connect(c_socket,(struct sockaddr *) &c_addr,sizeof(c_addr))==-1)
         {
                 printf("can not connect\n");
                 close(c_socket);
                 return -1;
         }
 
         printf("type> ");
         fgets(sendBuffer,sizeof(sendBuffer),stdin);
 
         write(c_socket,sendBuffer,sizeof(sendBuffer));
 
         n=read(c_socket,rcvBuffer,sizeof(rcvBuffer));
 
         rcvBuffer[n]='\0';
 
         printf("received : %s\n",rcvBuffer);
         return 0;
 
 
 }
