#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 점수를 입력하면 학점을 출력해주는 프로그램 작성
void get_grade(int score, char *grade){
	if(score >= 90)
		strcpy(grade, "A");
	else if (score >= 80)
		strcpy(grade, "B");
	else if (score >= 70)
		strcpy(grade, "C");
	else
		strcpy(grade, "F");
}

int main(int argc, char *argv[]){
	int score = 80;
	//실행 시 점수를 함께 입력 받기:
	if(argc < 2){
		printf("[Error] 실행 시 점수를 함께 입력하시오.\n");
		return 1;
	}
	score = atoi(argv[1]);
	char *grade = malloc(sizeof(char) * 10);
	
	get_grade(score, grade);
	printf("당신의 학점은 %s입니다.\n", grade);
	free(grade);
	return 0;
}
	
