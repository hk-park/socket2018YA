#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
void main()
{
    char *p=NULL;
    p=(char*)malloc(100);

    p[0]='\0';
    strcat(p,"hello");

    printf("%s\n",p);
    printf("%c\n",p[0]);
    free(p);
    p=NULL;
}
*/

void main()
{
    char *p=malloc(1);
    char buffer[11]="";
    int size=0,flag=0;

    while(fgets(buffer,10,stdin))
    {
	size += strlen(buffer);
	if(buffer[strlen(buffer)-1]=='\n')
	{
	    buffer[strlen(buffer)-1]='\0';
	    size--;
	    flag=1;
	}
	p=realloc(p,size+1);
	strcat(p,buffer);

	if(flag==1)
	    break;
	memset(buffer,'\0',11);
    }

    printf("%s\n",p);
    free(p);
}   
