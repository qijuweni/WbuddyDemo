/*************************************************************************
	> File Name: Salloc.h
	> Author: qijunwei
	> Mail: 708913847@163.com
	> Created Time: Mon 12 Dec 2016 01:50:59 PM CST

 ************************************************************************/
#ifndef SALLOC_H_
#define SALLOC_H_

#include "stdio.h"

struct SallocManager
{
    int orderNums_;
    char **pSallocArray_;
};

SallocManager* Init(int orderNums, char **pSallocArray);

void SetBit(char *pChar, int bitIndex, bool bIsSet1);

void SetBits(char *pChar, int endBitIndex, bool bIsSet1);

int IsFirstParaMax(char *pFirst, char *pScend, int charNums);

void SetChunkAllocted(SallocManager *pSallocManager, int order, unsigned int chunkNumb, char *pSetSalloc);
#endif
