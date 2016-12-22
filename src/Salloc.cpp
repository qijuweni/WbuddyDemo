/*************************************************************************
	> File Name: Salloc.h
	> Author: qijunwei
	> Mail: 708913847@163.com
	> Created Time: Mon 12 Dec 2016 01:50:59 PM CST

 ************************************************************************/

#include "Salloc.h"
#include <string.h>
#include <stdlib.h>

SallocManager* Init(int orderNums, unsigned char **pSallocArray)
{
    SallocManager *pSallocManager = (SallocManager*)malloc(sizeof(SallocManager));

    pSallocManager->orderNums_ = orderNums;
    pSallocManager->pSallocArray_ = pSallocArray;
    return pSallocManager;
}

void SetBit(unsigned char *pChar, int bitIndex, bool bIsSet1 = true)
{
    unsigned char mask = 1;
    mask = mask << bitIndex;

    if(bIsSet1 == true)
    {
        *pChar = (*pChar) | mask;
    }
    else
    {
        mask = ~mask;
        *pChar = (*pChar) & mask;
    }
}

void SetBits(unsigned char *pChar, int endBitIndex, bool bIsSet1 = true)
{
    unsigned char mask = 1;
    mask = (mask << (endBitIndex + 1)) - 1;

    if(bIsSet1 == true)
    {
        *pChar = (*pChar) | mask;
    }
    else
    {
        unsigned char tmp = 0xff;
        mask = tmp - mask;
        *pChar = (*pChar) & mask;
    }

}

int IsFirstParaMax(unsigned char *pFirst, unsigned char *pScend, int charNums)
{
    for(int i = charNums - 1; i >= 0; i--)
    {
        if((unsigned char)pFirst[i] > (unsigned char)pScend[i])
            return 1;
        else if((unsigned char)pFirst[i] < (unsigned char)pScend[i])
            return -1;
    }

    return 0;
}

void SetChunkAllocted(SallocManager *pSallocManager, int order, unsigned int chunkNumb, bool isAllocted, unsigned char *pSetSalloc)
{
    unsigned char *pSalloc = pSallocManager->pSallocArray_[order];
    int perChunkBits = order + 1;
    int modByChar = perChunkBits % 8;
    int perChunkSallocSpaceBytes = -1;

    if(modByChar != 0)
        perChunkSallocSpaceBytes = perChunkBits / 8 + 1;
    else
        perChunkSallocSpaceBytes = perChunkBits / 8;

    unsigned long OffsetByChar = perChunkSallocSpaceBytes * chunkNumb;
    unsigned char *pChunkSalloc = pSalloc + OffsetByChar;

    unsigned int broChunkNumb = -1;
    unsigned char *pBroChunkSalloc = NULL;
    int result = -2;

    if(order != pSallocManager->orderNums_ - 1)
    {
        broChunkNumb = (chunkNumb % 2) ? (chunkNumb - 1):(chunkNumb + 1);
        OffsetByChar = broChunkNumb * perChunkSallocSpaceBytes;
        pBroChunkSalloc =  pSalloc + OffsetByChar;

        result = IsFirstParaMax(pChunkSalloc, pBroChunkSalloc, perChunkSallocSpaceBytes);
    }

    if(pSetSalloc != NULL)
    {
//        strncpy(pChunkSalloc, pSetSalloc, pSetSallocLength);因为pSetSalloc 没多大，所以不必用函数
        int pSetSallocLength = 0;
        if((perChunkBits - 1) % 8 == 0)
            pSetSallocLength = perChunkSallocSpaceBytes -1;
        else
            pSetSallocLength = perChunkSallocSpaceBytes;


        if(pSetSalloc[pSetSallocLength - 1] == 0 )//意味着要修改最高位为1
        {
            if(pSetSallocLength == perChunkSallocSpaceBytes - 1)
            {
                if(isAllocted)
                    pChunkSalloc[perChunkSallocSpaceBytes - 1] =  1;
                else
                {
                    if(pChunkSalloc[perChunkSallocSpaceBytes -2] == 0)
                        pChunkSalloc[perChunkSallocSpaceBytes - 1] =  0;
                }

            }
            else
            {
                int bitSetIndex = -1;
                if(modByChar == 0)
                    bitSetIndex = 7;
                else
                    bitSetIndex = modByChar - 1;

                if(isAllocted)
                    SetBit(&pChunkSalloc[perChunkSallocSpaceBytes - 1], bitSetIndex, true);
                else if(pChunkSalloc[perChunkSallocSpaceBytes - 1] == (1 << bitSetIndex))
                    SetBit(&pChunkSalloc[perChunkSallocSpaceBytes - 1], bitSetIndex, false);
            }
        }

        for(int i = pSetSallocLength - 2; i >= 0; i--)
        {
            if(pChunkSalloc[i] == 0 && pSetSalloc[i] == 0)
            {
                break;
            }

            pChunkSalloc[i] = pSetSalloc[i];
        }

        if(pSetSallocLength == perChunkSallocSpaceBytes)
        {
            if(isAllocted)
           {
                pChunkSalloc[pSetSallocLength - 1] =  (pChunkSalloc[pSetSallocLength - 1] | pSetSalloc[pSetSallocLength - 1]);//=的话会有bug
           }
           else
           {
                unsigned char mask = 1;
                int tmp = 0;

                if(modByChar == 0)
                   tmp = 7;
                else
                   tmp = modByChar - 1;

                mask = (unsigned char)0xff - ((mask << tmp) - 1);
                pChunkSalloc[pSetSallocLength - 1] =  (pChunkSalloc[pSetSallocLength - 1] & (pSetSalloc[pSetSallocLength - 1] | mask));//=的话会有bug
            }
        }
        else
            pChunkSalloc[pSetSallocLength - 1] = pSetSalloc[pSetSallocLength - 1];

    }
    else
    {
        int setCharNum = -1;
        if(modByChar == 1)
        {
            if(isAllocted)
                pChunkSalloc[perChunkSallocSpaceBytes - 1] = 1;
            else
                pChunkSalloc[perChunkSallocSpaceBytes - 1] = 0;
        }
        else
        {
            int bitSetIndex = -1;
            if(modByChar == 0)
                bitSetIndex = 7;
            else
                bitSetIndex = modByChar - 1;

            if(isAllocted)
                SetBits(&pChunkSalloc[perChunkSallocSpaceBytes - 1], bitSetIndex, true);
            else
                SetBits(&pChunkSalloc[perChunkSallocSpaceBytes - 1], bitSetIndex, false);
        }

        setCharNum = perChunkSallocSpaceBytes - 1;
        for(int i = 0; i < setCharNum; i++)
        {
            if(isAllocted)
                pChunkSalloc[i] = 0xff;
            else
                pChunkSalloc[i] = 0x0;

        }
    }

    if(order != pSallocManager->orderNums_ - 1)
    {
        if(isAllocted)
        {
            if(result == -1 || (result == 0 && pBroChunkSalloc[perChunkSallocSpaceBytes - 1] == 0 ))
            {
                unsigned char *pUpSetChunk = pBroChunkSalloc;
                if(result != 0 && pSetSalloc != NULL)
                {
                    result = IsFirstParaMax(pChunkSalloc, pBroChunkSalloc, perChunkSallocSpaceBytes);

                    if(result == -1)
                        pUpSetChunk = pChunkSalloc;
                }
                SetChunkAllocted(pSallocManager, order + 1, chunkNumb / 2, isAllocted, pUpSetChunk);
            }
        }
        else
        {
            if(result == 1)
            {
                result = IsFirstParaMax(pChunkSalloc, pBroChunkSalloc, perChunkSallocSpaceBytes);

                if(result != 1)
                {
                    if(!(result == 0 && (pBroChunkSalloc[perChunkSallocSpaceBytes - 1] != 0)))
                        SetChunkAllocted(pSallocManager, order + 1, chunkNumb / 2, isAllocted, pChunkSalloc);
                }

            }
            else //我原来都比你小或者等于你，释放后肯定比你更小了
            {
                SetChunkAllocted(pSallocManager, order + 1, chunkNumb / 2, isAllocted, pChunkSalloc);
            }
        }
    }
}
