#include <stdio.h>
#define BUFFERSIZE 100

int main(){
	FILE *fp;
	char buffer[BUFFERSIZE];
	if ((fp = fopen("fileTxt", "r")) == NULL){
		printf("open failed\n");
		return -1;
	}
	while(fgets(buffer, 100, (FILE *)fp)){
		printf("%s", buffer);
	}
	if(!system("mkdir testDir"))
		printf("폴더생성 성공\n");
	else{
		printf("실패. 파일을 제거합니다.\n");
		system("rmdir testDir");
	}
	system("ls > list.txt");
}
