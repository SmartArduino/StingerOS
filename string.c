#include "string.h"

/*******************************************
* Copy string
* Input: destination, source, length of string
* Output: none
********************************************/
void string_copy(char *pDest, char *pSrc, int len){
	unsigned short length=len;

	while(length-- != 0){
		*pDest++ = *pSrc++;
	}
	*pDest='\0';
}

/*******************************************
* Get length of string
* Input: string
* Output: length of string
********************************************/
int string_length(char *pStr){
	char *pTmp;

	pTmp=pStr;
	while(*pStr != '\0'){
		pStr++;
	}

	return (pStr-pTmp);
}
/*******************************************
* Compare two string
* Input: string_1, string_2
* Output: 0,1,or -1
********************************************/
int string_compare(char *pSrc, char *pDst){
    int ret=0;

    while(!(*(unsigned char *)pSrc-*(unsigned char *)pDst) && *pDst){
		ret=*(unsigned char *)pSrc-*(unsigned char *)pDst;
        ++pSrc, ++pDst;
    }
    if(ret < 0){
        ret=-1;
    }else if(ret > 0){
        ret=1;
    }
    return ret;
}
