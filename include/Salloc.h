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
    unsigned char **pSallocArray_;
};

struct SallocManager* Init(int orderNums, unsigned char **pSallocArray);

void SetBit(unsigned char *pChar, int bitIndex, bool bIsSet1);

void SetBits(unsigned char *pChar, int endBitIndex, bool bIsSet1);

int IsFirstParaMax(unsigned char *pFirst, unsigned char *pScend, int charNums);

void SetChunkAllocted(SallocManager *pSallocManager, int order, unsigned int chunkNumb, bool isAllocted, unsigned char *pSetSalloc = NULL);
#endif
