/*************************************************************************
	> File Name: Salloc.h
	> Author: qijunwei
	> Mail: 708913847@163.com
	> Created Time: Mon 12 Dec 2016 01:50:59 PM CST

 ************************************************************************/

#include "Salloc.h"
#include <string.h>
#include <stdlib.h>

SallocManager* Init(int orderNums, char **pSallocArray)
{
    SallocManager *pSallocManager = (SallocManager*)malloc(sizeof(SallocManager));

    pSallocManager->orderNums_ = orderNums;
    pSallocManager->pSallocArray_ = pSallocArray;
    return pSallocManager;
}

void SetBit(char *pChar, int bitIndex, bool bIsSet1 = true)
{
    char mask = 1;
    if(bIsSet1 == true)
    {
        mask = mask << bitIndex;
    }

    *pChar = (*pChar) | mask;
}

void SetBits(char *pChar, int endBitIndex, bool bIsSet1 = true)
{
    char mask = 1;
    if(bIsSet1 == true)
    {
        mask = (mask << (endBitIndex + 1)) - 1;
    }

    *pChar = (*pChar) | mask;
}

int IsFirstParaMax(char *pFirst, char *pScend, int charNums)
{
    for(int i = charNums - 1; i >= 0; i--)
    {
        if(pFirst[i] > pScend[i])
            return 1;
        else if(pFirst[i] < pScend[i])
            return -1;
    }

    return 0;
}

void SetChunkAllocted(SallocManager *pSallocManager, int order, unsigned int chunkNumb, char *pSetSalloc = NULL)
{
    char *pSalloc = pSallocManager->pSallocArray_[order];
    int perChunkBits = order + 1;
    int modByChar = perChunkBits % 8;
    int perChunkSallocSpaceBytes = -1;

    if(modByChar != 0)
        perChunkSallocSpaceBytes = perChunkBits / 8 + 1;
    else
        perChunkSallocSpaceBytes = perChunkBits / 8;

    unsigned long OffsetByChar = perChunkSallocSpaceBytes * chunkNumb;
    char *pChunkSalloc = pSalloc + OffsetByChar;

    unsigned int broChunkNumb = (chunkNumb % 2) ? (chunkNumb - 1):(chunkNumb + 1);
    OffsetByChar = broChunkNumb * perChunkSallocSpaceBytes;
    char *pBroChunkSalloc =  pSalloc + OffsetByChar;

    int result = IsFirstParaMax(pChunkSalloc, pBroChunkSalloc, perChunkSallocSpaceBytes);

    if(pSetSalloc != NULL)
    {
//        strncpy(pChunkSalloc, pSetSalloc, pSetSallocLength);因为pSetSalloc 没多大，所以必用调用函数
        int pSetSallocLength = 0;
        if((perChunkBits - 1) % 8 == 0)
            pSetSallocLength = perChunkSallocSpaceBytes -1;
        else
            pSetSallocLength = perChunkSallocSpaceBytes;

        for(int i = pSetSallocLength - 1; i >= 0; i--)
        {
            pChunkSalloc[i] = pSetSalloc[i];
            if(pSetSalloc[i] == 0)
            {
                break;
            }
        }

        if(pSetSalloc[pSetSallocLength - 1] == 0)//意味着要修改最高位为1
        {
            if(pSetSallocLength == perChunkSallocSpaceBytes - 1)
            {
                pChunkSalloc[perChunkSallocSpaceBytes - 1] =  1;
            }
            else
            {
                int bitSetIndex = -1;
                if(modByChar == 0)
                    bitSetIndex = 7;
                else
                    bitSetIndex = modByChar - 1;

                SetBit(&pChunkSalloc[perChunkSallocSpaceBytes - 1], bitSetIndex);
            }
        }
    }
    else
    {
        int setCharNum = -1;
        if(modByChar == 1)
        {
            pChunkSalloc[perChunkSallocSpaceBytes - 1] = 1;
        }
        else
        {
            SetBits(&pChunkSalloc[perChunkSallocSpaceBytes - 1], modByChar - 1);
        }

        setCharNum = perChunkSallocSpaceBytes - 1;
        for(int i = 0; i < setCharNum; i++)
            pChunkSalloc[i] = 0xff;
    }
/*
    if(result == 1)
    {
    }
    else if(result == -1)
    {
        SetChunkAllocted(pSallocManager, order + 1, chunkNumb / 2, pBroChunkSalloc);
    }
    else if(result == 0)
    {
        if(*pBroChunkSalloc == 0)
        {

        }
    }
*/
}
