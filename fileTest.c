#include<stdio.h>

#define BUFSIZE 100

int main(){
	FILE *fp;
	char buffer[BUFSIZE];

	fp = fopen("test.txt", "r");
	if(fp){
		while(fgets(buffer, BUFSIZE, (FILE*)fp)){
			printf("%s", buffer);
		}
	}else{
		printf("not found\n");
	}

	//system() example
	//int result = system("mkdir testdir");
	int result = system("ls > list.txt");
	if(!result){
		printf("success\n");
	}else{
		printf("failed\n");
	}
	return 0;
}
