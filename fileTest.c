#include <stdio.h>

#define BUFSIZE 100

int main() {
	FILE *fp;
	char buffer[BUFSIZE];
	fp = fopen("test.txt", "r");
	if(fp)	{
		while(fgets(buffer, BUFSIZE, (FILE *)fp))
		printf("%s", buffer);
	} else {
		printf("파일을 찾을 수 없습니다.\n");
	}

	//system() 함수 예제
	int result = system("ls > list.txt");
	if(!result)	printf("명령어가 정상적으로 수행되었습니다.\n");
	else		printf("명령어의  수행이 실패하였습니다.\n");	

	return 0;
}
