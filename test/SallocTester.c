/*************************************************************************
	> File Name: SallocTester.c
	> Author: qijunwei
	> Mail: 708913847@163.com
	> Created Time: Mon 12 Dec 2016 10:45:11 PM CST

 ************************************************************************/
#include "stdio.h"
#include <gtest/gtest.h>
#include "Salloc.h"
#include <string.h>

#define PAGE_SIZE 4096
#define SPACE  4096 * 256 * 4 * 256

static struct SallocManager* SallocManagerForTest()
{
    int MaxChunkNumb = SPACE / PAGE_SIZE;
    int orderNums = 1;

    int tmpChunkNumb = MaxChunkNumb;
    while(tmpChunkNumb/2 != 0)
    {
        orderNums++;
        tmpChunkNumb = tmpChunkNumb / 2;
    }
    unsigned char **pSallocArr = (unsigned char**)malloc(sizeof(char*) * orderNums);

    for(int i = 0; i < orderNums; i++)
    {
        int bits = i + 1;
        int perSallocSize = (bits % 8 ) ? bits / 8 + 1 : bits / 8;
        pSallocArr[i] = (unsigned char*)malloc(perSallocSize * MaxChunkNumb);
        memset(pSallocArr[i], 0, perSallocSize * MaxChunkNumb);
        MaxChunkNumb = MaxChunkNumb / 2;
    }

    return Init(orderNums, pSallocArr);
}

//2^x个page时，则 orderNums = x + 1, 例如一个page对应的orderNum为1，对应的位向量为1位

static SallocManager* pManager = SallocManagerForTest();

TEST(Salloc, SetBit)
{
    unsigned char a = 6;
    SetBit(&a, 3, true);
    EXPECT_EQ(a, 14);

    unsigned char b = 0;
    SetBit(&b, 0, true);
    EXPECT_EQ(b, 1);

    unsigned char c = 7;
    SetBit(&c, 7, true);
    EXPECT_EQ(c, (unsigned char)135);

    unsigned char d = 15;
    SetBit(&d, 2, false );
    EXPECT_EQ(d, 11);

    unsigned char e = 255;
    SetBit(&e, 7, false);
    EXPECT_EQ(e,255 - 128);
}

TEST(Salloc, SetBits)
{
    unsigned char a = 6;
    SetBits(&a, 3, true);
    EXPECT_EQ(a, 15);

    unsigned char b = 0;
    SetBits(&b, 0, true);
    EXPECT_EQ(b, 1);

    unsigned char c = 7;
    SetBits(&c, 7, true);
    EXPECT_EQ(c, (unsigned char)255);

    unsigned char d = 255;
    SetBits(&d, 7, false);
    EXPECT_EQ(d, 0);

    unsigned char e = 23;
    SetBits(&e, 2, false);
    EXPECT_EQ(e, 16);
}

TEST(Salloc, IsFirstParaMax)
{
    unsigned char a[2] = {0};
    a[1] = (unsigned char)128 + 64;
    unsigned char b[2] = {0};
    b[1] = (unsigned char)128;

    EXPECT_EQ(1,IsFirstParaMax(a, b, 2));

    a[0] = 12;
    b[0] = 8;
    EXPECT_EQ(1,IsFirstParaMax(a, b, 1));
}


TEST(Salloc, SetChunkAllocted)
{
//这个test是在 SetChunkAllocted 没有加上递归修改功能的测试代码
 /*   SetChunkAllocted(pManager, 2, 3, true);
    EXPECT_EQ(pManager->pSallocArray_[2][3], 7);

    SetChunkAllocted(pManager, 0, 14, true);
    EXPECT_EQ(pManager->pSallocArray_[0][14], 1);

    SetChunkAllocted(pManager, 1, 5, true);
    EXPECT_EQ(pManager->pSallocArray_[1][5], 3);

    SetChunkAllocted(pManager, 3, 1, true);
    EXPECT_EQ(pManager->pSallocArray_[3][1], 15);

    SetChunkAllocted(pManager, 9, 0, true);//因为order9对应10bit的位向量，所以一个位向量占两个字节
    EXPECT_EQ(pManager->pSallocArray_[9][1], 3);//下标越大，代表越高位的字节
    EXPECT_EQ(pManager->pSallocArray_[9][0], (unsigned char)255);

    SetChunkAllocted(pManager, 8, 1, true);//同上
    EXPECT_EQ(pManager->pSallocArray_[8][3], 1);
    EXPECT_EQ(pManager->pSallocArray_[8][2], (unsigned char)255);

    SetChunkAllocted(pManager, 4, 5, true);//模拟的是order4的第6个组（下标为5）被用，然后第5个组可以用，这时候需要向上更新
    SetChunkAllocted(pManager, 5, 2, &pManager->pSallocArray_[4][4]);
    EXPECT_EQ(pManager->pSallocArray_[5][2], (unsigned char)32);

    SetChunkAllocted(pManager, 10, 0, &pManager->pSallocArray_[9][2]);//模拟order为9的第一个组（下标为0，位向量占两个字节）被用了，第二组可用，向上更新
    EXPECT_EQ(pManager->pSallocArray_[10][1], (unsigned char)4);
    EXPECT_EQ(pManager->pSallocArray_[10][0], (unsigned char)0);

    SetChunkAllocted(pManager, 7, 2, true);//模拟的是order7的第3个组（下标为2）被用，然后第4个组可以用，这时候需要向上更新
    SetChunkAllocted(pManager, 8, 1, &pManager->pSallocArray_[7][3]);//为了走到if(pSetSallocLength == perChunkSallocSpaceBytes - 1)
    EXPECT_EQ(pManager->pSallocArray_[8][3], (unsigned char)1);
    EXPECT_EQ(pManager->pSallocArray_[8][2], (unsigned char)0);
*/

    SallocManager *pSalloc = SallocManagerForTest();

    SetChunkAllocted(pSalloc, 0, 0, true);

    unsigned char value = 1;
    int num = pSalloc->orderNums_;
    for(int i = 0; i < 8; i++)
    {
        if( i >= num )
            break;

        EXPECT_EQ(pSalloc->pSallocArray_[i][0], (unsigned char)value);
        value *= 2;
    }

    value = 1;
    for(int i = 8; i < 16; i++)
    {
        if( i >= num )
            break;

        EXPECT_EQ(pSalloc->pSallocArray_[i][1], (unsigned char)value);
        EXPECT_EQ(pSalloc->pSallocArray_[i][0], (unsigned char)0);
        value *= 2;
    }

    value = 1;
    for(int i = 16; i < 24; i++)
    {
        if( i >= num )
            break;

        EXPECT_EQ(pSalloc->pSallocArray_[i][2], (unsigned char)value);
        EXPECT_EQ(pSalloc->pSallocArray_[i][1], (unsigned char)0);
        EXPECT_EQ(pSalloc->pSallocArray_[i][0], (unsigned char)0);

        value *= 2;
    }

    SetChunkAllocted(pSalloc, 2, 1, true);
    EXPECT_EQ(pSalloc->pSallocArray_[3][0], (unsigned char)12);
    EXPECT_EQ(pSalloc->pSallocArray_[4][0], (unsigned char)16);
/*
    SetChunkAllocted(pSalloc, 3, 1, true);
    EXPECT_EQ(pSalloc->pSallocArray_[4][0], (unsigned char)28);
    EXPECT_EQ(pSalloc->pSallocArray_[5][0], (unsigned char)32);
*/
    SetChunkAllocted(pSalloc, 2, 2, true);
    EXPECT_EQ(pSalloc->pSallocArray_[3][1], (unsigned char)8);
    EXPECT_EQ(pSalloc->pSallocArray_[4][0], (unsigned char)24);

    SetChunkAllocted(pSalloc, 4, 1, true);
    EXPECT_EQ(pSalloc->pSallocArray_[5][0], (unsigned char)56);
    EXPECT_EQ(pSalloc->pSallocArray_[6][0], (unsigned char)64);

    SetChunkAllocted(pSalloc, 7, 1, true);
    EXPECT_EQ(pSalloc->pSallocArray_[8][0], (unsigned char)128);
    EXPECT_EQ(pSalloc->pSallocArray_[8][1], (unsigned char)1);

 /*   SetChunkAllocted(pSalloc, 7, 3, true);
    EXPECT_EQ(pSalloc->pSallocArray_[8][3], (unsigned char)1);
    EXPECT_EQ(pSalloc->pSallocArray_[8][2], (unsigned char)0);
    EXPECT_EQ(pSalloc->pSallocArray_[9][1], (unsigned char)3);
*/
    SetChunkAllocted(pSalloc, 8, 1, true);
    EXPECT_EQ(pSalloc->pSallocArray_[9][0], (unsigned char)128);
    EXPECT_EQ(pSalloc->pSallocArray_[9][1], (unsigned char)3);

    SetChunkAllocted(pSalloc, 0, 0, false);
    EXPECT_EQ(pSalloc->pSallocArray_[0][0], (unsigned char)0);
    EXPECT_EQ(pSalloc->pSallocArray_[1][0], (unsigned char)0);
    EXPECT_EQ(pSalloc->pSallocArray_[2][0], (unsigned char)0);
    EXPECT_EQ(pSalloc->pSallocArray_[3][0], (unsigned char)8);
    EXPECT_EQ(pSalloc->pSallocArray_[4][0], (unsigned char)24);
    EXPECT_EQ(pSalloc->pSallocArray_[5][0], (unsigned char)56);

    SetChunkAllocted(pSalloc, 2, 1, false);
    EXPECT_EQ(pSalloc->pSallocArray_[0][0], (unsigned char)0);
    EXPECT_EQ(pSalloc->pSallocArray_[4][0], (unsigned char)16);
    EXPECT_EQ(pSalloc->pSallocArray_[5][0], (unsigned char)48);

}

