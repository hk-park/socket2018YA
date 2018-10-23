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
//in_ source, in_ target
int soc_msgcmp(char *source, char* target)
{
    if(source==NULL || target == NULL)
	exit(1);
    return !strncasecmp(source,target,strlen(target));
}

//in_ src, in_ limit, out_ arrCnt, in_ stripNewline
char** soc_strsplit(char *src,char *limit, int *arrCnt)
{
    //init 
    *arrCnt=0;
    char *pstr=src;
    int limitIdx=0;
    char **ppStr=NULL;
    int i=0,j=0,srcIdx=0,splitPos=0,splitLen=0;
    int flag=0;
    int endIdx=0;

    
    for(limitIdx=0;limitIdx<strlen(limit);limitIdx++)
    {
        if(src[strlen(src)-1]==limit[limitIdx])
        {
	    flag=1;
	    endIdx=strlen(src)-1;
	    break;
	}
    }

    if(!flag)	
	endIdx=strlen(src);

    //get token cnt
    while(srcIdx<=endIdx)
    {
	for(limitIdx=0;limitIdx<strlen(limit);limitIdx++)
	{
	    if(src[srcIdx]==limit[limitIdx] || srcIdx==endIdx)
	    {
		(*arrCnt)++;
		break;
	    }
	}


	srcIdx++;
    }

    //build array
    flag=0;
    srcIdx=0;
    limitIdx=0;
    ppStr=malloc(sizeof(char*) * (*arrCnt));
    while(1)
    {
	for(limitIdx=0;limitIdx<strlen(limit);limitIdx++)
	{
	    flag=0;

	    if(src[srcIdx]==limit[limitIdx] || srcIdx==endIdx)
	    {
		flag=1;

		ppStr[i]=malloc(sizeof(char) * splitLen);

		for(j=0;j<splitLen;++j)
		{
		    ppStr[i][j]=src[splitPos + j];
		}

	    	ppStr[i][j]='\0';
		splitPos += splitLen + 1;
		i++;
		splitLen = 0;
		break;
	    }
	}

	if(srcIdx==endIdx)
	    break;

	if(!flag)
	    splitLen++;

	srcIdx++;
    }



    //debug
    //for(i=0;i<*arrCnt;++i)
    //	printf("%s\n",ppStr[i]);

    return ppStr;
}

//in_ ppStr, in_ cnt
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

//in_ socket,in_ msg
void soc_write(int socket,char *msg)
{
    write(socket,msg,strlen(msg));
}


#endif

