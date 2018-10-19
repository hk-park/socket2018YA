#ifndef __MY_UTILS__
#define __MY_UTILS__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

/*	    SOCKET2018YA LIBRARY	    */
//All functions must be prefixed with 'soc_'
//'soc_' represents namespace of socket2018YA
//////////////////////////////////////////////

/*	    GLOBALs	    */



/*	    FUNCs	    */
//wrapper of strncasecmp
int soc_msgcmp(char *source, char* target)
{
    if(source==NULL || target == NULL)
	exit(1);
    return !strncasecmp(source,target,strlen(target));
}

//in_ src, in_ limit, out_ arrStr, out_ arrCnt
char** soc_strsplit(char *src,char *limit, int *tokenCnt)
{
    //init 
    *tokenCnt=0;
    char *pstr=src;
    char *plimit;
    char **ppStr=NULL;
    int i=0,j=0;
    int len=0;
    int flag=0;
    int tmp=0;


    //get token cnt
    while(*pstr!='\0')
    {
	for(plimit=limit;*plimit!='\0';plimit++)
	{
	    if(*pstr==*plimit)
	    {
		(*tokenCnt)++;
		break;
	    }
	}

	pstr++;
    }

    pstr=src;

    //create token array
    ppStr=malloc(sizeof(char*) * (*tokenCnt));
    while(*pstr!='\0')
    {

	for(plimit=limit;*plimit!='\0';plimit++)
	{
	    flag=0;
	    if(*pstr==*plimit)
	    {
		ppStr[i]=malloc(sizeof(char) * len);
		for(j=0;j<len;++j)
		{
		    ppStr[i][j]=src[tmp+j];
		}
		ppStr[i][j]='\0';
		tmp+=len+1;
		flag=1;
		i+=1;
		len=0;
		break;
	    }
	}

	if(flag==0)
	    len++;
	

	pstr++;
    }

    for(tmp=0;tmp<i;++tmp)
    {
	printf("%s\n",ppStr[tmp]);
    }

    return ppStr;
}

void soc_freeCharPtrPtr(char** ppStr,int cnt)
{
    if(ppStr==NULL)
	return;

    int i=0;
    for(i=0;i<cnt;++i)
    {
	free(ppStr[i]);
    }
    free(ppStr);
    
}


void soc_write(int socket,char *msg)
{
    write(socket,msg,strlen(msg));
}

void soc_end()
{

}

#endif

