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

//wrapper for strncasecmp
//in_ source, in_ target
int soc_msgcmp(char *source, char* target)
{
    int result=0;

    if(source==NULL || target == NULL)
	exit(-1);
    return (result=strncasecmp(source,target,strlen(target)) ) ? 0 : 1;
}

int soc_msgcmp_full(char *source,char *target)
{
    int result=0;
    if(source==NULL || target == NULL)
	exit(-1);
    return (result=strncasecmp(source,target,strlen(target)) ) ? 0 : 1;
}

//in_ src, in_ limit, out_ strCnt
char** soc_strsplit(char *src,char *limit, int *strCnt)
{
    //init 
    *strCnt=0;
    char *pstr=src;
    int limitIdx=0;
    char **ppStr=NULL;
    int i=0,j=0,srcIdx=0,splitPos=0,splitLen=0;
    int flag=0;
    int endIdx=0;

    endIdx=strlen(src);
    for(limitIdx=0;limitIdx<strlen(limit);limitIdx++)
    {
        if(src[endIdx-1]==limit[limitIdx])
        {
	    flag=1;
	    endIdx--;
	    break;
	}
    }

    if(flag==0 && src[endIdx-1]=='\n')
	endIdx--;
    flag=0;

    //get token cnt
    while(srcIdx<=endIdx)
    {
	for(limitIdx=0;limitIdx<strlen(limit);limitIdx++)
	{
	    if(src[srcIdx]==limit[limitIdx] || srcIdx==endIdx)
	    {
		(*strCnt)++;
		break;
	    }
	}
	srcIdx++;
    }

    if(*strCnt<=1)
	return NULL;

    //build array
    srcIdx=0;
    limitIdx=0;
    ppStr=malloc(sizeof(char*) * (*strCnt));

    while(srcIdx<=endIdx)
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

	if(!flag)
	    splitLen++;

	srcIdx++;
    }

    //debug
    //for(i=0;i<*strCnt;++i)
    //	printf("%s\n",ppStr[i]);

    return ppStr;
}

char * soc_strBuildFromArray(char **ppStr,int arrCnt,int offset)
{
    char *pBuild=NULL;
    int len=0;
    int i=0;

    if(ppStr==NULL)
	return;

    for(i=offset;i < arrCnt;++i)
    {
	//문자열 뒤에 공백 추가하기위해 +1
	len+=strlen(ppStr[i])+1;
    }

    //널문자를 위해 +1
    pBuild=malloc(len+1);

    for(i=offset;i < arrCnt;++i)
    {
	strcat(pBuild,ppStr[i]);
	strcat(pBuild," ");
    }

    pBuild[len]='\0';
    
    return pBuild;
}

//out_ pppStr, in_ cnt
void soc_freeCharPtrPtr(char*** pppStr,int cnt)
{
    int i=0;
    if(pppStr==NULL)
	return;

    for(i=0;i<cnt;++i)
    {
	free((*pppStr)[i]);
    }
    free(*pppStr);
    *pppStr=NULL;
}

//out_ ppStr
void soc_freeCharPtr(char** ppStr)
{
    if(ppStr==NULL)
	return;

    free(*ppStr);
    *ppStr=NULL;
}

//in_ fd,in_ msg
void soc_write(int fd,char *msg)
{
    write(fd,msg,strlen(msg));
}


#endif

