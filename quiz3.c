#include <stdio.h>

//학생의 이름, 나이, 학번 정보를 담고 있는 "student" 구조체를 선언하고 
//학생 정보를 출력하는 프로그램 구현

struct student{
	int snum; //학번
	char name[50]; //이름
	int age; //나이
};

int main(){
	struct student std;
	scanf("%d %s %d", &std.snum, &std.name, &std.age);
	printf("학번: %d\n이름: %s\n나이: %d\n", std.snum, std.name, std.age);
	return 0;
}
