#ifndef QUAKESHAREDBUFFERS_H
#define QUAKESHAREDBUFFERS_H

#include <vector>
#include <cstdint>

#define blockSize 8192
#define sharedBufferSize (blockSize + 1)

struct ShareBuffer;

extern std::vector<ShareBuffer*>* buffers;

struct ShareBuffer {
    int32_t id = -1;
    int32_t blockNum = -1;
    std::vector<uint8_t> data;
    int32_t count = -1; // usage count
    int32_t pinCount = -1; 
    int32_t state =-1;
    
    ShareBuffer();
};

void initSharedBuffers(int32_t bufferSize);
void addBuffer(int32_t id, int32_t blockNum, std::vector<uint8_t> data, int32_t count,int32_t state);

#endif