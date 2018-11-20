#include <stdio.h>

//1~100 사이에 있는 x의 배수를 출력하는 프로그램 구현

int main(){
	int x = 5;
	printf("배수를 출력할 x값을 입력하시오: ");
	scanf("%d", &x);
	
	for(int i = 1; i*x <= 100; i++){
		printf("%d X %d = %d\n", x, i, x*i);
	}
	return 0;
}
