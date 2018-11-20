#include <stdio.h>

// 점수를 입력하면 학점을 출력해주는 프로그램 작성
char get_grade(int score){
	if(score >= 90)
		return 'A';
	else if (score >= 80)
		return 'B';
	else if (score >= 70)
		return 'C';
	else
		return 'F';
}

int main(){
	int score = 80;
	scanf("%d", &score);
	char grade;
	
	grade = get_grade(score);
	printf("당신의 학점은 %c입니다.\n", grade);
	return 0;
}
	
