/*************************************************************************
	> File Name: ChunkGroup.h
	> Author: qijunwei
	> Mail: 708913847@163.com
	> Created Time: Sun 11 Dec 2016 06:44:13 PM CST

 ************************************************************************/
#ifndef CHUNKGROUP_H_
#define CHUNKGROUP_H_

class ChunkGroup
{
    public:
        ChunkGroup(int order, unsigned int chunkNum);
        ~ChunkGroup();
        unsigned int GetChunk();

    private:
        unsigned int *pulArrayOfChunkUsedCount;
        char *pcGroupSalloc;
}

#endif
