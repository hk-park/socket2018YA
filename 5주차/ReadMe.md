# ../lib/utils.h
#### int soc_msgcmp(char *source, char* target) : strncasecmp 랩퍼
#### void soc_write(int socket,char *msg) : write 랩퍼
#### char** soc_strsplit(char *src,char *limit, int *tokenCnt) : strtok 함수를 간편하게 사용하기 위한 나만의 함수를 만듬
#### void soc_freeCharPtrPtr(char** ppStr,int cnt) : soc_strsplit 함수를 사용 후 메모리 해제를 위한 함수

# soc_strsplit 예제
https://github.com/hk-park/socket2018YA/blob/%EC%95%88%EC%9E%AC%ED%98%95/5%EC%A3%BC%EC%B0%A8/server5.c#L106

