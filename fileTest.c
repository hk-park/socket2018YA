#include <stdio.h>
#define BUFSIZE 100

int main(){
	FILE* fp;
	char buffer[BUFSIZE];

	fp = fopen("test.txt", "r");
	if(fp){
		while(fgets(buffer, BUFSIZE, (FILE*) fp))
			printf("%s\n", buffer);
	}
	else{
		printf("파일을 찾을 수 없습니다.\n");
	}

	int result = system("mkdir testdir");
	if(!result)
		printf("명령어가 정상적으로 수행되었습니다.\n");
	else
		printf("명령어 수행에 실패했습니다.");

	return 0;
}
