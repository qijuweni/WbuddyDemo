/*************************************************************************
	> File Name: ChunkGroup.h
	> Author: qijunwei
	> Mail: 708913847@163.com
	> Created Time: Sun 11 Dec 2016 07:24:35 PM CST

 ************************************************************************/
#ifndef CHUNKGROUP_H_
#define CHUNKGROUP_H_

struct ChunkGroup
{
    unsigned int *pulArrayOfChunkUsedCount;
    char *pcGroupSalloc;
    int order;
    unsigned int chunkNum;
};

ChunkGroup* InitChunkGroup(int order, unsigned int chunkNum);

unsigned int GetFitChunkNumbInBuddyChunks(ChunkGroup *pChunkGroup, unsigned int beginChunkNumb,int wantedOrder);


void SetChunkUesd(ChunkGroup *pChunkGroup, unsigned int ChunkNumb);

#endif
