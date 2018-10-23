#include <stdio.h>

#define BUFSIZE 100
int main(){
	FILE *fp; //파일을 읽기 위한 포인터 선언
	char buffer[BUFSIZE];
	fp = fopen("test.txt", "r"); //파일 읽기 위해서 fopen 함수 호출하고 그걸 fp 포인터에 저장
	
	if(fp){
		while(fgets(buffer, BUFSIZE, (FILE *)fp)) //파일을 읽어오는 것. 계속 읽기 위해 while 추가
			printf("%s", buffer);
	}else{ //파일 포인터가 null이면
		printf("파일을 찾을 수 없습니다.\n");
	}
	
	//system() 함수 예제
	int result = system("ls > list.txt");
	//int result = system("mkdir testdir"); //해당 명령어 수행. testdir 생성함.
	if(!result)
		printf("명령어가 정상적으로 수행되었습니다.\n");
	else
		printf("명령어 수행이 실패하였습니다.\n");	
	return 0;
}
