#include <stdio.h>
#define BUFSIZE 100
int main()
{
	FILE * fp;
	char buffer[BUFSIZE];
	fp = fopen("test.txt", "r"); //url of the file, mode(read, write..etc) ex)rw
	//file pointer
	if(fp)
	{
		while(fgets(buffer,BUFSIZE , (FILE *)fp))//read data from file/ read single(one) line
		
		printf("%s\n", buffer);
	}else{
		printf("cannot find file\n");
	}
		
	//system() example
	int result = system("mkdir testdir"); 
	//int result = system("ls > list.txt");
	if(!result) //if result = 0
		printf("명령어가 정상적으로 수행되었습니다. \n");
	else
		printf("명령어가 정상적으로 수행되지 않았습니다\n");

	return 0;

		


}
