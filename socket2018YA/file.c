#include<stdio.h>

#define BUFSIZE 100
int main(){
	FILE *fp;
	char buffer[BUFSIZE];
	fp = fopen("test.txt","r");
	
	if(fp){
		while(fgets(buffer, BUFSIZE, (FILE *)fp))
			printf("%s",buffer);
		
	}else{
		printf("not found\n");
	}
	int result = system("mkdir testdir");
	if(!result)
		printf("1\n");
	else
		printf("0\n");
	return 0;
}
