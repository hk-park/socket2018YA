#include<stdio.h>

int main(){
  int pid;
  int a = 10;
  int b = 20;

  a++;
  pid = fork();
  //pid = 자식 프로세스의 pid값 if 부모 프로세스
  //pid = 0 if 자식 프로세스
  //pid = -1 if fork() 함수 실패
  if(pid > 0){ // 부모프로세스
    a++;
    printf("[PARENT] a = %d, b = %d\n", a, b);
  }else if(pid==0){ //자식프로세스
    b++;
    printf("[CHILD] a = %d, b = %d\n", a, b);
  }else{ //실패
    printf("fail");
  }
}
