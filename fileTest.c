#include <stdio.h>
<<<<<<< HEAD


#define BUFSIZE 100

int main(){
	FILE *fp;
	char buffer[100];
	fp = fopen("test.txt", "r");
	//fopen함수 리턴값은 파일을 열 수 있는 포인터이기 때문에 포인터 변수 선언 후 대입

	if(fp){
			while(fgets(buffer, BUFSIZE, (FILE *)fp))
				//fgets는 파일을 읽을 수 있으면 1 없으면 0이다.
				printf("%s", buffer);
	} else{
			printf("파일을 찾을 수 없습니다.\n");
	}
	//system()함수 예제
	int result = system("mkdir testdir");
	//system() 리눅스 콘솔에서 명령어 치는 것과 동일
	// ls > list.txt를 수행해서 리스트목록을 텍스트로 저장가능
	if(!result)
			printf("명령어가 정상적으로 수행되었습니다.  \n");
	else
			printf("명령어 수행이 실패하였습니다. \n");

	return 0;

}	
	
=======
#include <string.h>

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
>>>>>>> b939c5761fa1e7aabc12d42d114c8f967a3bcb48
