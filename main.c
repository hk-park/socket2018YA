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
		while(fgets(buffer, 100, (FILE *)fp))//read data from file/ read single(one) line
		
		printf("%s\n", buffer);
	}else{
		printf("cannot find file\n");
	}
	return 0;

		


}
